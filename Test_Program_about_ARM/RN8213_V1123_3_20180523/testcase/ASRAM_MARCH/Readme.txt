1. 上电等待P16，P15=0b11
2. 执行用例
4. 测量LCDVD,LCDVC,LCDVB,LCDVA。
6. 执行失败，则置P10～P12=0b010
   执行成功，则置P10～P12=0b101
   
   测试结束，程序死循环