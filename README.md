---

# STM32_ServoCar：基于STM32与ELRS协议的智能遥控小车

<div align="center">

![舵机小车-成品方案](https://github.com/user-attachments/assets/4d346ed2-bfcf-4fc4-a57d-d0aa356fcc80)

**全流程自主开发的智能遥控小车｜从电路仿真到机械结构设计的一体化实践**

[![STM32](https://img.shields.io/badge/MCU-STM32F103C8T6-blue)](https://www.st.com/)
[![ELRS](https://img.shields.io/badge/Protocol-ELRS-orange)](https://www.expresslrs.org/)
[![PCB Design](https://img.shields.io/badge/PCB-Cadence%2F%E7%AB%8B%E5%88%9BEDA-green)]()

</div>

## 📋 项目概述

本项目是一个完全自主开发的智能遥控小车，旨在通过完整的硬件产品开发流程，系统性地实践嵌入式系统、电路设计和机械结构相关知识。

### 核心参数
- **主控芯片**：STM32F103C8T6
- **通信协议**：ELRS（ExpressLRS）2.4G，实现高响应遥控
- **硬件设计**：Proteus仿真验证 → Cadence/立创EDA绘制原理图与PCB → 嘉立创打样生产
- **结构设计**：SolidWorks完成整车三维建模与结构优化

### ✨ 项目亮点
- ✅ **全流程自主开发**：从电路仿真、嵌入式开发到机械结构设计，实现产品完整研发流程
- ✅ **双版本迭代优化**：V1立创EDA入门验证 → V2 Cadence专业设计，形成清晰的迭代改进路径
- ✅ **完整的文档体系**：包含采购清单、电路原理图、PCB设计文件、3D模型及源码

---

## 🔄 版本演进

本项目经历了两个主要版本的迭代：

<div align="center">
<img src="https://github.com/user-attachments/assets/b1041458-c37c-476a-adde-8c57d3f3c5b6" width="700" alt="实物照片"/>
<p><em>实物照片：第一版PCB兼容两代车架</em></p>
</div>

### 第一版：麦轮小车
- 采用四个N20减速电机，通过差速控制实现全向移动
- 使用立创EDA完成PCB设计（适合初学者快速上手）
- 验证了ELRS通信协议和基础运动控制逻辑

### 第二版：舵机转向小车
- 保留第一版PCB的兼容性，仅更换车架结构
- 采用SG90舵机实现前轮转向 + 后轮驱动
- 使用Cadence重新设计PCB，优化布局和电源保护
- SolidWorks完成整车结构优化，3D打印制作车架

---

## 📦 采购清单

| 组件 | 规格/型号 | 数量 | 备注 |
|------|-----------|------|------|
| ELRS接收机 | 2.4G 100mW | 1 | 串口通信 |
| 锂电池 | 7.4V 2S | 1 | 供电推荐 |
| N20减速电机 | 6V 200RPM（推荐） | 4 | 4电机驱动 |
| 轮胎 | 14mm橡胶胎 | 4 | 适配舵机版小车 |
| SG90舵机 | 9g微型舵机 | 1 | 转向机构 |
| 螺丝 | M3*10 | 若干 | 固定结构件 |
| PCB板 | 定制 | 1 | BOM表详见下方 |

> 📌 完整BOM表可在`00_BOM`文件夹中下载

---

## 💻 嵌入式开发

### 开发环境
- **IDE**：STM32CubeIDE（HAL库）
- **调试工具**：ST-Link

<div align="center">
<img src="https://github.com/user-attachments/assets/e0fb8fa1-5037-400e-a9e9-879a4ec633f4" width="700" alt="CubeIDE配置"/>
<p><em>STM32CubeIDE引脚功能配置</em></p>
</div>

### 代码结构

#### 舵机与电机控制 (`sk_motor.c`)
```c
void Servo_Init(void);           // 舵机初始化（PWM配置）
void Servo_Move(uint8_t CHn);    // 舵机转向控制，输入0~100（对应-90°~+90°）
void Motor_Init(void);            // 电机驱动初始化（GPIO+PWM）
void Motor_Move(uint8_t CHn, uint8_t Start);  // 电机调速，输入0~100
```

#### ELRS协议解析 (`elrs.c`)
```c
uint16_t ELRS_CRC8(uint8_t *Data);                // CRC校验（多项式0xD5，初始值0x00）
void ELRS_DataAnalysis(uint16_t *Channel, uint8_t *ReceiveData);  // 解析ELRS负载数据，提取CH1~CH10
uint16_t ELRS_ToPercentage(uint16_t CHn, uint16_t Min, uint16_t Max);  // 通道值归一化（0~100）
void ELRS_Process(uint8_t *DataA, uint8_t *CH);   // 主处理函数：原始数据输入→CH1~CH10输出（0~100）
```

<div align="center">
<img src="https://github.com/user-attachments/assets/e903373c-c7ef-4a57-a3f0-54db5703d7c1" width="700" alt="代码结构"/>
<p><em>详细代码查看如上目录</em></p>
</div>

### 📡 ELRS通信协议详解

**串口配置**：
- USART1（PA9-TX, PA10-RX）
- 波特率：420Kbits
- 数据格式：8N1
- 供电：5V

**数据包结构**：

| 字节位置 | 内容 | 说明 |
|---------|------|------|
| 0 | 同步字节 | 固定为`0xC8` |
| 1 | 长度 | 类型`1`+负载`22`+CRC`1`=`24`（十六进制`0x18`） |
| 2 | 类型 | 表示数据帧的类型，RC通道数据类型为`0x16` |
| 3~24 | 负载 | CH1~CH16，每个通道11bit，共176位（22字节） |
| 25 | CRC校验 | 校验范围：字节2~24 |

> CRC算法：多项式`0xD5`，初始值`0x00`，用于数据完整性验证

### 🔧 Proteus仿真

在PCB打样前，使用Proteus搭建仿真电路，验证STM32最小系统、电机驱动逻辑。

<div align="center">
<img src="https://github.com/user-attachments/assets/2b670c7a-8984-4922-959f-2d4414153742" width="700" alt="Proteus仿真"/>
<p><em>Proteus电路仿真验证</em></p>
</div>

---

## 🔌 硬件设计

### 电源功耗预算

| 组件 | 工作电压 | 典型电流 | 峰值电流 | 备注 |
|------|---------|---------|---------|------|
| N20电机（单个） | 6V | 100mA | 200mA | 堵转电流 |
| SG90舵机 | 5V | 200mA | 500mA | 堵转电流 |
| ELRS接收机 | 5V | 20mA | 30mA | - |
| TB6612FNG（双路） | 5V | 5mA | 10mA | 逻辑部分 |
| STM32F103 | 3.3V | 50mA | 150mA | - |
| **总计** | - | **~500mA** | **~1.5A** | 按四个电机 |

### 版本V1：立创EDA入门验证

作为PCB设计的入门实践，第一版采用立创EDA完成，重点掌握原理图绘制、PCB布局布线、BOM导出和样板焊接调试。

#### 关键元件参数

| 参考文档 | 型号 | 封装 | 功能 | 关键参数 |
|-----|------|------|------|---------|
| B1 | LDL1117S33R | SOT-223 | 3.3V LDO | 输入2.5-18V，输出3.3V/1.2A，压差350mV@1.2A |
| B2 | LDL1117S50R | SOT-223 | 5V LDO | 输入2.5-18V，输出5V/1.2A，压差600mV@1.2A |
| C3 | TB6612FNG | SSOP-24 | 电机驱动 | 逻辑5V，驱动15Vmax，1.2A(ave)/3.2A(peak) |

#### 设计文件

<div align="center">
<img src="https://github.com/user-attachments/assets/15246239-8d40-4a61-8bee-ccfc28bd290a" width="700" alt="V1原理图"/>
<p><em>V1原理图（立创EDA）</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/2b40056d-955d-42c2-b7d1-695782a05a1e" width="700" alt="V1 PCB布局"/>
<p><em>V1 PCB布局布线</em></p>
</div>

#### BOM表

<div align="center">
<img src="https://github.com/user-attachments/assets/07e1952e-0d63-4cfc-a70f-bb75f8438da6" width="700" alt="V1 BOM"/>
<p><em>V1 BOM表（可在00_BOM文件夹下载）</em></p>
</div>

#### 实物成品

<div align="center">
<img src="https://github.com/user-attachments/assets/65cab5bb-fae3-4814-8826-55714c82de0c" width="700" alt="V1 PCB成品"/>
<p><em>V1 PCB打样成品</em></p>
</div>


#### 版本V1经验教训

1. **布局不合理**：电机接线端口集中在单侧，导致小车走线混乱，线束整理困难
2. **缺少保护机制**：无过流/过压保护，若电源模块击穿会损坏全板
3. **踩坑教训**：采购廉价芯片导致反复排查，延误开发进度约两周

### 版本V2：Cadence专业设计

基于V1的经验教训，使用Cadence进行完全重新设计，引入更专业的EDA工具链。

#### 工具链学习
- **OrCAD Capture**：原理图设计、元器件符号绘制、网表和BOM导出
- **Padstack Editor**：焊盘和过孔定制
- **Allegro PCB Designer**：PCB布局布线、封装绘制、规则设置

#### 关键改进

1. ✅ **布局优化**：电机输出端口按实际需求调整，便于走线整理
2. ✅ **过流保护**：增加保险丝，防止电源短路损坏
3. ✅ **电源模块重构**：优化电源走线，提升接入电压上限
4. ✅ **电源开关**：增加总电源控制，方便调试


#### 新增关键元件

| 参考文档 | 型号 | 封装 | 功能 | 关键参数 |
|-----|------|------|------|---------|
| B3 | MP1584EN-LF-Z | SOIC-8-EP | DCDC降压 | 输入7-28V，输出0.8-25V，3A，静态电流100uA |


#### 原理图设计

<div align="center">
<img src="https://github.com/user-attachments/assets/b644a857-5b70-4dd2-858d-53f1c33646b2" width="700" alt="V2原理图1"/>
<p><em>V2原理图 - 设计思路</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/235d66d5-fcc6-4e03-8305-610d2147ca20" width="700" alt="V2原理图2"/>
<p><em>V2原理图 - MCU</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/95a1dac7-f889-41be-a299-354de79da981" width="700" alt="V2原理图3"/>
<p><em>V2原理图 - POWER</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/988e0a6e-f454-4928-8ca3-48f9f3646f44" width="700" alt="V2原理图4"/>
<p><em>V2原理图 - 电机驱动</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/b1fc4fc1-f354-4d16-bb38-1ca511ffc203" width="700" alt="V2原理图5"/>
<p><em>V2原理图 - 对外接口</em></p>
</div>

#### PCB设计

<div align="center">
<img src="https://github.com/user-attachments/assets/49185e57-54b1-4ef2-aa29-4827c9dc98fc" width="700" alt="V2 PCB顶层"/>
<p><em>V2 PCB - 顶层布线</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/37544024-853f-4634-8343-7e5232415cb0" width="700" alt="V2 PCB GND层"/>
<p><em>V2 PCB - GND层</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/a555a96f-7786-4701-ab8e-6bd355a942a8" width="700" alt="V2 PCB电源层"/>
<p><em>V2 PCB - 电源层</em></p>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/09230fb6-6f06-4ab0-b88a-78b21b7f9aa0" width="700" alt="V2 PCB双层视图"/>
<p><em>V2 PCB - 底层</em></p>
</div>

#### BOM表

<div align="center">
<img src="https://github.com/user-attachments/assets/e9b94fbf-467c-4cde-b758-a0a274f38d20" width="700" alt="V2 BOM"/>
<p><em>V2 BOM表</em></p>
</div>




---

## 🏗️ 机械结构设计

### SolidWorks 3D建模

使用SolidWorks完成整车结构设计，包括底盘承载布局、舵机安装机构、电池仓设计和螺丝固定位优化。

<div align="center">
<img src="https://github.com/user-attachments/assets/4d346ed2-bfcf-4fc4-a57d-d0aa356fcc80" width="700" alt="SolidWorks模型"/>
<p><em>SolidWorks整车三维模型</em></p>
</div>

### 3D打印实现

从SolidWorks导出STL文件，导入Blender进行模型预览与颜色配置。

<div align="center">
<img src="https://github.com/user-attachments/assets/51c274cc-9090-4e10-b5de-cf8e81455ce9" width="700" alt="Blender渲染"/>
<p><em>Blender渲染效果图</em></p>
</div>

**打印参数**：
- **设备**：拓竹P1S（0.4mm喷头）
- **材料**：PLA/ PETG

**设计注意事项**：
- 打印误差补偿：PLA/PETG普遍存在0.1~0.2mm误差，设计需预留公差
- 螺丝孔优化：若需拧入M3螺丝，设计孔径**2.95mm**可获得最佳紧固效果

---

## 📚 参考目录

| 编号 | 文档名称 | 说明 |
|-----|---------|------|
| 【A1】 | STM32F103x8B数据手册 | 主控芯片数据手册 |
| 【A2】 | STM32F10xxx参考手册 | 开发参考手册 |
| 【A3】 | STM32F103C8T6引脚定义 | 引脚功能定义 |
| 【B1】 | LDL1117S33R规格书 | 3.3V LDO数据手册 |
| 【B2】 | LDL1117S50R规格书 | 5V LDO数据手册 |
| 【B3】 | MP1584EN-LF-Z规格书 | DCDC电源芯片数据手册 |
| 【C3】 | TB6612FNG规格书 | 电机驱动芯片数据手册 |

---

## 📝 补充资料

### 麦轮运动学分析

麦轮小车的运动控制基于四个麦克纳姆轮的转速组合：

<div align="center">
<img src="https://github.com/user-attachments/assets/e4d0dc25-e124-47a4-a848-cefe1ae2499c" width="600" alt="麦轮解析1"/>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/cdded9bf-9756-46fe-86dd-78ab072fe0e0" width="600" alt="麦轮解析2"/>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/8847cdd3-5ee3-4a16-acd6-485b63c4b5c7" width="600" alt="麦轮解析3"/>
<p><em>麦轮运动学解析</em></p>
</div>

运动模式：
- **平移**：四轮同向同速
- **横移**：对角线轮同向，另一对角线反向
- **旋转**：左侧与右侧反向

### N20减速电机特性

<div align="center">
<img src="https://github.com/user-attachments/assets/0d30a9fd-ea05-41aa-a658-50e35fde4925" width="600" alt="N20电机参数"/>
<p><em>N20减速电机6V供电参数图</em></p>
</div>

6V供电下，200RPM版本：
- 空载电流：约60mA
- 推荐工作电流：50-150mA
- 堵转电流：约200mA

---

## 📬 联系我

- **邮箱1**：1161812370@qq.com
- **邮箱2**：shang_ah@sina.com

**如果这个项目对你有帮助，欢迎Star⭐！**

---

> **项目状态**：持续完善中  
