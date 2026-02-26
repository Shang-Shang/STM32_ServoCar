# STM32_ServoCar
项目名称：基于STM32与ELRS协议的智能遥控小车  
主控芯片：STM32F103C8T6  
通信方案：采用ELRS（ExpressLRS）协议实现高响应遥控  
硬件设计：Proteus仿真验证电路 → Cadence绘制原理图与PCB → 嘉立创打样生产  
结构设计：SolidWorks完成整车三维建模与结构优化  
项目亮点：从电路仿真、嵌入式开发到机械结构设计全流程自主完成，具备完整的硬件产品开发能力  

<img width="3840" height="2088" alt="舵机小车-成品方案" src="https://github.com/user-attachments/assets/4d346ed2-bfcf-4fc4-a57d-d0aa356fcc80" />

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

## Solidworks

### 3D打印中的误差问题

### 模块验证

## 3D打印产品
从solidworks中导出stl文件，导入blender中，配置颜色进行预览
<img width="3840" height="2088" alt="舵机小车-成品渲染" src="https://github.com/user-attachments/assets/51c274cc-9090-4e10-b5de-cf8e81455ce9" />


## 补充
### 麦轮小车方案运动轨迹计算
![麦轮轨道解析1](https://github.com/user-attachments/assets/e4d0dc25-e124-47a4-a848-cefe1ae2499c)
![麦轮轨道解析2](https://github.com/user-attachments/assets/cdded9bf-9756-46fe-86dd-78ab072fe0e0)
![麦轮轨道解析3](https://github.com/user-attachments/assets/8847cdd3-5ee3-4a16-acd6-485b63c4b5c7)

