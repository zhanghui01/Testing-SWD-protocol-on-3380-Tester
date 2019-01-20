2015-5-25 更换ADS1243  调整ADC系数
2016-07-29  SocPumpEna()LCDPUMP测试前，增加LCD IO配置函数。
2016-07-29  ANA_BGR由0x6158改为0x615a。（改善ACD线性，详情见高飞邮件）
             《eeprom.h  Line 21:ANA_BGR由0x6158改为0x615a。/ra0_routine.c  Line 339:(标准片8211读RTC_TEMP前要读ANA_BGR,这里要兼容)》

2016-07-29  LCDpump测试前配置SEG。（待定）
               <adc.c  Line 160-228,加入uint32_t SocLcdIoCfg( void )函数;/LCD_pump.c  Line35:加入SocLcdIoCfg();>


