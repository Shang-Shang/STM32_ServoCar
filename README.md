# STM32_ServoCar
项目名称：基于STM32与ELRS协议的智能遥控小车  
主控芯片：STM32F103C8T6  
通信方案：采用ELRS（ExpressLRS）协议实现高响应遥控  
硬件设计：Proteus仿真验证电路 → Cadence绘制原理图与PCB → 嘉立创打样生产  
结构设计：SolidWorks完成整车三维建模与结构优化  
项目亮点：从电路仿真、嵌入式开发到机械结构设计全流程自主完成，具备完整的硬件产品开发能力  

<img width="3840" height="2088" alt="舵机小车-成品方案" src="https://github.com/user-attachments/assets/4d346ed2-bfcf-4fc4-a57d-d0aa356fcc80" />

![IMG_20260227_225554](https://github.com/user-attachments/assets/b1041458-c37c-476a-adde-8c57d3f3c5b6)


## 采购清单


## Proteus仿真
CubeIDE生成HAL软件
<img width="3840" height="2088" alt="功能配置" src="https://github.com/user-attachments/assets/e0fb8fa1-5037-400e-a9e9-879a4ec633f4" />

## HAL IDE程序编写
最终版本的驱动程序用CubeIDE HAL库编写，早先写的标准库版本亦上传

### ELRS信号解析
ELRS 接收机采用串口通信，本项目使用STM32F103C8T6芯片，具体配置为：

* USART1（PA9，PA10）
* 420Kbits，8N1
* CRC校验，多项式为0xD5，初始值为0x00
* VCC：5V

### OLED显示解析



### 舵机转向方案舵机配置

## Cadance 原理图及PCB绘制

### 电路功耗计算：

* N20_1000 减速电机：6V供电时，堵转电流 200mA，四个电机共800mA
* 舵机sg90，堵转电流约500mA
* ELRS接收机：100mW，5V供电，电流约20mA
* TB6612FNG电机驱动芯片，逻辑区5V供电，电流约5mA，两块共10mA
* STM32芯片最大电流约150mA
总最大电流约 1.5A

减速电机6V供电，参数如下图
![Screenshot_20260228_143254_com_taobao_taobao_TTDetailActivity_edit_66536109915085](https://github.com/user-attachments/assets/0d30a9fd-ea05-41aa-a658-50e35fde4925)



### 版本V1 —— 立创EDA绘制
网上有很多立创EDA的高质量课程，是入门PCB设计的理想选择。因此我选择从立创EDA入门，设计第一块PCB板。  
针对麦轮小车，采用立创EDA设计。通过这个板子的设计，对PCB板的原理图绘制，元件布局布线，BOM导出采购，PCB样板下单，PCB焊接和虚焊检测等等有了切实的实践。
V1 原理图绘制：
<img width="1850" height="1312" alt="image" src="https://github.com/user-attachments/assets/15246239-8d40-4a61-8bee-ccfc28bd290a" />
V1 PCB板绘制如下
<img width="3840" height="2088" alt="image" src="https://github.com/user-attachments/assets/2b40056d-955d-42c2-b7d1-695782a05a1e" />
V1 BOM表如下（可以在00 BOM文件夹中下载）
<img width="3641" height="1356" alt="image" src="https://github.com/user-attachments/assets/07e1952e-0d63-4cfc-a70f-bb75f8438da6" />
V1 PCB 成品：
<img width="1190" height="636" alt="image" src="https://github.com/user-attachments/assets/65cab5bb-fae3-4814-8826-55714c82de0c" />


关键元器件参数：
* LDL1117S33R【B1】：封装SOT-223，LDO，输入2.5到18V，输出3.3V，1.2A，压差350mV@1.2A，静态电流250uA
* LDL1117S50R【B2】：封装SOT-223，LDO，输入2.5到18V，输出5V，1.2A，压差600mV@1.2A，静态电流250uA
* TB6612FNG【C3】：封装SSOP-24，逻辑输入5V，电机驱动输入15Vmax，输出电流1.2A(ave)/3.2A(peak)

版本V1存在的问题：
1. 元件布局不符合实际需求：例如电机的供电线都集中在一侧，与两侧的电机布线不配合，电机电线不好整理  
2. 电源模块没有保险机制，若电源模块被击穿，16V电压会直接作用在电路所有的元件上，损坏整块电路板。  
3. 根据STM32原理图【A1-P23】  
4. 贪小便宜吃大亏，买便宜芯片结果不能用，反复确认板子 + 来回快递用了半个多月才解决问题。  

### 版本V2 —— Cadance绘制

版本V2进行的改进：
1. 电机输出位尽心调整
3. 通过3A保险丝实现过流保护
4. 重新设计电源模块，
5. 增加电源开关

通过Cadance，学习了OrCad，Padstack，Allegro。学习了OrCad的原理图绘制，元器件绘制，网表和BOM导出。学习了Padstack的焊盘，和各种过孔绘制。学习了Allegro的元器件封装绘制，PCB板绘制，布线规则设置，以及软件的使用技巧。可以根据元器件图纸，用orcad绘制元件原理图符号，padstack和allegro绘制其焊盘和封装，对Cadance软件使用有了认识和心得。

关键元器件参数：
* MP1584EN-LF-Z【B3】：封装SOIC-8-EP，DCDC降压，输入7到28V，输出0.8到25V，3Amax，静态电流100uA
* 

V2 原理图绘制：
![V2 PCB_页面_1](https://github.com/user-attachments/assets/b644a857-5b70-4dd2-858d-53f1c33646b2)
![V2 PCB_页面_3](https://github.com/user-attachments/assets/235d66d5-fcc6-4e03-8305-610d2147ca20)
![V2 PCB_页面_4](https://github.com/user-attachments/assets/95a1dac7-f889-41be-a299-354de79da981)
![V2 PCB_页面_5](https://github.com/user-attachments/assets/988e0a6e-f454-4928-8ca3-48f9f3646f44)
![V2 PCB_页面_6](https://github.com/user-attachments/assets/b1fc4fc1-f354-4d16-bb38-1ca511ffc203)


V2 PCB板绘制：
<img width="3840" height="2088" alt="V2 PCB Top" src="https://github.com/user-attachments/assets/49185e57-54b1-4ef2-aa29-4827c9dc98fc" />
<img width="3840" height="2088" alt="V2 PCB GND" src="https://github.com/user-attachments/assets/37544024-853f-4634-8343-7e5232415cb0" />
<img width="3840" height="2088" alt="V2 PCB POWER" src="https://github.com/user-attachments/assets/a555a96f-7786-4701-ab8e-6bd355a942a8" />
<img width="3840" height="2088" alt="V2 PCB Top Bottom" src="https://github.com/user-attachments/assets/09230fb6-6f06-4ab0-b88a-78b21b7f9aa0" />


V2 BOM表：
<img width="2096" height="1158" alt="image" src="https://github.com/user-attachments/assets/e9b94fbf-467c-4cde-b758-a0a274f38d20" />




## Solidworks

### 3D打印中的误差问题

### 模块验证

## 3D打印产品
从solidworks中导出stl文件，导入blender中，配置颜色进行预览
<img width="3840" height="2088" alt="舵机小车-成品渲染" src="https://github.com/user-attachments/assets/51c274cc-9090-4e10-b5de-cf8e81455ce9" />
  
3D打印设备：拓竹P1S 0.4mm喷头
3D打印误差：PLA和PETG普遍有0.1~0.2mm的打印误差，设计时应加以注意
螺丝孔误差：若希望拧入3mm螺丝，应在solidworks中设计孔洞直径为2.95mm，可以保证有较好的拧紧效果。


## 参考目录
【A1】STM32F103x8B数据手册  
【A2】STM32F10xxx参考手册  
【A3】STM32F103C8T6引脚定义  
  
【B1】C435835_线性稳压器(LDO)_LDL1117S33R_规格书_WJ134515  
【B2】C970558_线性稳压器(LDO)_LDL1117S50R_规格书_WJ315897  
【B3】C15051_DC-DC电源芯片_MP1584EN-LF-Z_规格书_WJ1074595  
  
【C3】C88224_有刷直流电机驱动芯片_TB6612FNG(O,C,8,EL_规格书_WJ137724  



## 补充
### 麦轮小车方案运动轨迹计算
![麦轮轨道解析1](https://github.com/user-attachments/assets/e4d0dc25-e124-47a4-a848-cefe1ae2499c)
![麦轮轨道解析2](https://github.com/user-attachments/assets/cdded9bf-9756-46fe-86dd-78ab072fe0e0)
![麦轮轨道解析3](https://github.com/user-attachments/assets/8847cdd3-5ee3-4a16-acd6-485b63c4b5c7)

