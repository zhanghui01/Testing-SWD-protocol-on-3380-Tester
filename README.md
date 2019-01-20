# Testing-SWD-protocol-on-3380-Tester
关于SWD协议在3380D测试机的测试记录

SWD协议部分解析：

SWD共四个接口，Vcc,GND,SWDIO,SWDCLK

备注：SWD其实就2根线的协议，SWDCLK控制时钟,SWDIO控制数据，通过对寄存器的读写来完成于MCU的通信。
首先，需要进入SWD模式。至少发50个1 和一个0xE79E（适合Cortex M0 32bit处理器,注意：一定是低位先发，有些文章说高位先发送是错误的，官方文档就是低位数据先发送） 强制切换到SWD模式即可。J-link切换时通常还会发一个0x6DB7，据说是兼容老版本ARM的强制切换


SWD时钟说明：SWD时钟在下降沿时采集数据，即时钟在由高变低时，数据位要保持稳定，否则有可能无法采样！时钟在由低变高时进行数据的锁存！！


#数据头为8bit的数据->start,ApnDp,RnW/RnR,A[2:3],Parity校验位,Stop(0),Park(1);数据头后面的Trn可以忽略，直接进入ACK
#-Start     起始位，始终为1，这也是Target判断总线从空闲状态退出的条件。
#-APnDP  	选择要访问的是DP(0)寄存器还是AP(1)寄存器。
#-Rnw    选择是读(1)还是写(0)。
#-A[2:3]   DP或者AP寄存器的地址，注意它是低位在前。比如DP寄存器 Select 它的地址为0x08，那这儿的A为C(0b1000)，显然A[2:3]就为01。
#					DP寄存器
#					Address     Read              Write
#					0x00        IDCODE           ABORT
#					0x04        CTRL/STAT        CTRL/STAT
#					0x08        RESEND           SELECT
#					0x0C        RDBUFF           N/A
#					AP寄存器
#					Address     Read              Write
#					0x00         CSW               CSW
#					0x04         TAR               TAR
#					0x08         N/A               N/A
#					0x0C         DRW               DRW
#					0xFC         IDR               N/A
#-Praity   校验位，它是APnDP、RnW和A[2:3]共4个bit的校验位。奇数为1，偶数为0
#-Stop    停止位。始终为0。
#-Park     该位确切来说应该始终为1，ADI V5中描述此位由总线上拉，但由于总线的上拉能力不足，会导致Target识别不了这个1。该勘误在ADI V5.2中有说明。

#读命令为：数据头+Trn+ACK+RDATA+Parity 构成，有文章指出Trn可以忽略，是因为时钟没有设置好，一个周期内最好先有下降沿再有上升沿，保证在一个周期内对数据进行采样和锁存。

#写命令为：数据头+Trn+ACK+Trn+WDATA+Parity，在这不同的是，在写命令时必须要考虑2个Trn的位置。


# 数据是下降沿放出，上升沿锁存（输入输出都一样）
# 复位之后必须首先读取DP ID，之后向DP的0x4寄存器写0x50000000，使能debug
#数据头为0xA5,读取的数据应为0x0BB11477；即数据头为0xA9,写数据为0x50000000
# 向DP的0x8寄存器写0xF0设定AP bank值，然后读取AP的0xC寄存器，可以得到AP ID
# 即数据头为0xB1,写数据0x00000000，选择AP Bank0，
# 注意AP是POST属性，所以需要读取2次才能得到正确的值
# 读出AP的0x0寄存器（CSW），将最后三个比特改为b010，使能32位访问（注意要先将DP的0x8写为0）
# 然后向AP的0x4地址写0x40034002C（针对本次项目的RN SOC），从AP的0xC读取device ID
#例如：
进行Linreset后，给如下指令：
0xA5,0x0BB11477//获取DP IDCODE
0x81,0x0000001E//write clear abort
0xA9,0x50000000//write enable debug
0x8D,0xF0000000//read ,ensure enable dubug  
0xB1,0x00000000//write,select AP bank0            
0x87,0x00000000//read,check AP CSW
0xBD,0x03000040//read,read DP 0x0C
0xA3,0x80000042//write,enable BANK0  32bit data
0x8B,0x4003402C//write,write CHIP_ID address to ap
0x9F,0x00000000//read,read AP IDR
0x9F,data	//read Again,read AP CHIP_ID = 0x00008219

#Memory read:
#  1. Read AP-CSW(Write DP-Select; Read AP) check if AP is busy
#  2. Write AP-CSW to enable AP(SW debug, AP enable)
#  3. Write AP-TAR to set AP memory address
#  4. Read AP-DRW to get data from AP memory looply.
#  PS: max. length of read data is: TAR ~ address[31:10] | 0x3ff (1K)







1.对DUT的SWD进行复位，连接DUT的SWD时需要进行LineReset，测试机需要连续发送50个"1",
使得DUT进行Line Reset,之后至少插入2个Idle,然后可以读取DUT的DP的ID，判断DUT的类型。

第一步：进行Line_reset，数据口发送50个1后再发送0xE79E，强制进入SWD模式,再至少发送2个Idle，即数据口为0；
进行Line_reset后第一次必须读取的是DP寄存器的0x00寄存器，即读取DP的ICCODE，数据头为0xA5,判定读取的数据是否为0x0BB11477(数据头设定读DP寄存器，DP寄存器为0x0BB11477);

第二步：Power_up，进行上电操作，写入数据0x50000000(操作指令为（0xA9,0x50000000）);

第三步：选择AP寄存器，操作指令为0xB1,0x00000000

第四步：写指令，使得AP寄存器bank0是32位数据操作，指令为：0xA3,0x80000042

第五步：通过AP寄存器传送地址0x4003402C，操作指令为：0x8B,0x4003402C；

第六步：读DUT的ID，地址为0x4003402C，判断数据是否为对应型号的ID（0x8219），操作指令为0x9F,0x00000000

第七步：再读DUT的ID（需要都两次才有数据出现），判断数据是否为对应型号的ID（0x8219），操作指令为0x9F,0x00008219

第八步：写指令，使得AP寄存器bank0是16位数据操作，指令为：0xA3,0x80000041

第九步：写指令，通过AP寄存器传送地址0x08C0008C，操作指令为：0x8B,0x08C0008C

第十步: 读DUT的CP_RESLUT的结果，地址为0x08C0008C(flash的地址)，判断数据是否为对应型号的CP_RESULT（0x1234），操作指令为0x9F,0x00000000

第十一步：再读DUT的CP_RESLUT的结果，地址为0x08C0008C(flash的地址)，判断数据是否为对应型号的CP_RESULT（0x1234），操作指令为0x9F,0xFFFF1234
