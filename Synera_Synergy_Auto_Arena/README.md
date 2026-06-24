# Synera: Synergy Auto-Arena

> C++ 自走棋风格 PvE 游戏 | Qt 6.11 + CMake | OOP 课程项目

---

## 1. 项目简介

Synera: Synergy Auto-Arena 是一款受《The Last Flame》《Teamfight Tactics》启发的**自动战斗策略游戏**。玩家在棋盘上布置英雄，利用**种族与职业羁绊**、**装备搭配**和**神兽祝福**来对抗一波波腐化敌人。

- 8×8 六边形棋盘，4 大区域 × 4 小关 + 最终 Boss，共 17 场 PvE 战斗
- 5 种族（卓拉/利特/格鲁德/鼓隆/海拉鲁）× 5 职业（战士/射手/法师/刺客/守卫）
- 15 位英雄，每位拥有独特技能
- 4 种装备 + 4 神兽祝福
- 商店招募 → 布阵 → 自动战斗 → 结算 → 下一关 循环

---

## 2. 构建与运行

### 环境要求

| 依赖 | 版本 |
|------|------|
| Qt | 6.11.0（MinGW 64-bit） |
| CMake | 3.16+ |
| 编译器 | MinGW-w64 或 MSVC 2019+ |
| 系统 | Windows 10/11 |

### 构建步骤

```bash
# 1. 进入项目目录
cd Synera_Synergy_Auto_Arena

# 2. CMake 配置
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# 3. 编译
cmake --build build --config Release

# 4. 运行
./build/Synera_Synergy_Auto_Arena.exe
```

> 也可直接用 **Qt Creator** 打开 `CMakeLists.txt`，选择 MinGW 64-bit Kit，点击运行。

---

## 3. 游戏规则

### 3.1 核心概念

| 概念 | 说明 |
|------|------|
| **Board（棋盘）** | 8×8 六边形格子，上半为敌方，下半为己方 |
| **Bench（备战区）** | 棋盘下方 8 格，用于存放待上场英雄 |
| **Unit（单位）** | 英雄，拥有 HP / ATK / Range / Max Mana / Mana 属性 |
| **Owner** | `PlayerCtrl`（己方）或 `EnemyCtrl`（敌方） |
| **Race（种族）** | 卓拉 / 利特 / 格鲁德 / 鼓隆 / 海拉鲁 / Boss |
| **Profession（职业）** | 战士 / 射手 / 法师 / 刺客 / 守卫 |
| **State** | Idle → Moving → Attacking → Casting → Dead |
| **Star（星级）** | 1★～3★，3 个同星英雄自动合成升星 |

### 3.2 羁绊系统

场上己方英雄达到数量阈值时激活羁绊效果：

| 种族 | 2 单位 | 4 单位 |
|------|--------|--------|
| 海拉鲁 | 全体生命 +150 | 全体生命 +300 |
| 卓拉 | 开局获得 20 法力 | 开局获得 40 法力 |
| 鼓隆 | 全体最大生命 +10% | 全体最大生命 +20% |
| 格鲁德 | 全体攻击力 +10 | 全体攻击力 +20 |
| 利特 | 全体攻速 +10% | 全体攻速 +20% |

| 职业 | 2 单位 | 3 单位 |
|------|--------|--------|
| 战士 | 全体攻击力 +5 | 生命低于 50% 时伤害 +30% |
| 射手 | 攻击距离 +1 | 每攻击 3 次，第 4 次 150% 伤害 |
| 法师 | 最大法力 -20 | 技能命中眩晕目标 0.5 秒 |
| 刺客 | 攻速 +15% | 首次攻击 200% 伤害 |
| 守卫 | 全体生命 +100 | 开局获得一次伤害免疫 |

### 3.3 装备系统

| 装备 | 效果 |
|------|------|
| **Sword（剑）** | ATK +15 |
| **Mail（铠甲）** | Max HP +150 |
| **Gloves（手套）** | 攻速 +20% |
| **Crystal（水晶）** | 最大法力 -30 |

每单位最多携带 1 件装备。击败敌人后有概率掉落装备。

### 3.4 神兽祝福

每通过一个区域（第 4/8/12/16 关），获得对应神兽的永久祝福：

| 神兽 | 激活时机 | 效果 |
|------|----------|------|
| 瓦·露塔（水） | 通过卓拉区域 | 全体生命 +60 |
| 瓦·梅德（风） | 通过利特区域 | 全体攻速 +10% |
| 瓦·娜波力斯（雷） | 通过格鲁德区域 | 全体最大法力 -10 |
| 瓦·鲁达尼亚（火） | 通过鼓隆区域 | 全体攻击力 +10 |

### 3.5 法力值与技能

- 起始法力为 0，每次普攻 +10
- 法力达到最大值时自动施放技能
- 技能施放后法力归零，进入冷却

### 3.6 胜负条件

- 己方全部阵亡 → 失败，扣除生命值（5～20 点）
- 敌方全部阵亡 → 胜利，获得金币 +4、经验 +3
- 玩家 HP 降至 0 → 游戏结束

---

## 4. 项目结构

```
Synera_Synergy_Auto_Arena/
├── CMakeLists.txt              # CMake 构建配置
├── resources.qrc               # Qt 资源文件（图片等）
├── app.rc                      # Windows 应用图标
│
├── assets/
│   ├── logo.ico / logo.png     # 应用图标
│   └── units/                  # 20 个英雄 PNG 贴图（32×32 像素）
│
├── include/
│   ├── core/
│   │   ├── Board.h             # 棋盘类（8×8 + 寻路辅助）
│   │   └── Game.h              # 游戏主逻辑（回合/战斗/商店/羁绊/拖拽）
│   ├── entity/
│   │   ├── Unit.h              # 单位基类（属性/状态机/技能虚函数）
│   │   ├── Heroes.h            # 15 个英雄子类声明
│   │   └── Player.h            # 玩家数据（HP/金币/等级/神兽祝福）
│   └── gui/
│       ├── GameWindow.h        # 主窗口（顶层 UI 布局）
│       ├── GridItem.h          # 六边形/方形格子渲染
│       ├── UnitItem.h          # 单位渲染（贴图/血条/法力条/拖拽）
│       ├── InfoPanel.h         # 右侧信息面板（属性/商店/羁绊/装备）
│       ├── EquipmentItem.h     # 装备图标 + 拖拽
│       ├── EquipmentSlotItem.h # 装备槽
│       ├── PauseMenuPanel.h    # 暂停菜单
│       ├── SettlementPanel.h   # 回合结算面板
│       └── StartMenuWidget.h   # 主菜单/开始界面
│
├── src/
│   ├── main.cpp                # 程序入口
│   ├── core/
│   │   ├── Board.cpp           # 棋盘实现
│   │   └── Game.cpp            # 游戏主逻辑实现
│   ├── entity/
│   │   ├── Unit.cpp            # 单位实现（状态机/寻路/法力/战斗）
│   │   ├── Heroes.cpp          # 15 个英雄技能实现
│   │   └── Player.cpp          # 玩家数据实现
│   └── gui/
│       ├── GameWindow.cpp      # 主窗口实现
│       ├── GridItem.cpp        # 格子渲染
│       ├── UnitItem.cpp        # 单位渲染
│       ├── InfoPanel.cpp       # 信息面板
│       ├── EquipmentItem.cpp   # 装备图标
│       ├── EquipmentSlotItem.cpp
│       ├── PauseMenuPanel.cpp
│       ├── SettlementPanel.cpp
│       └── StartMenuWidget.cpp
│
├── 单位.md / 单位.html         # 英雄数值设计文档
└── 数值设计.md / 数值设计.html  # 系统数值设计文档
```

---

## 5. 操作说明

| 操作 | 方式 |
|------|------|
| 购买英雄 | 点击商店栏中的英雄卡片（消耗 3 金币） |
| 出售英雄 | 选中英雄后点击 InfoPanel 的出售按钮 |
| 上/下场 | 拖拽单位到棋盘/备战区 |
| 交换位置 | 拖拽单位到已有单位的格子上 |
| 装备 | 拖拽装备图标到己方单位上 |
| 购买经验 | 点击"升级"按钮（4 金币换 4 XP） |
| 刷新商店 | 点击"刷新"按钮（2 金币） |
| 开始战斗 | 点击"开战"按钮 |
| 暂停 | 点击"暂停"按钮 |
| 存档/读档 | 暂停菜单中操作 |

---

## 6. 设计亮点

- **OOP 继承体系**：`Unit` 虚基类 → 15 个英雄子类，`castSkill()` / `updateUnit()` 多态重写
- **状态机驱动**：FSM（Idle → Moving → Attacking → Casting → Dead）统一管理所有单位行为
- **六边形棋盘**：BFS 寻路 + 距离范围计算
- **Qt 信号槽解耦**：Model（Game/Unit/Player）与 View（GUI 组件）通过 signals/slots 通信
- **存档系统**：文本序列化玩家状态、单位信息、装备，支持断点续玩
- **羁绊 + 祝福 + 装备**：三层 Buff 叠加系统，战斗前自动计算

---

## 7. AI 使用声明

本项目在开发过程中使用了 AI（Claude、ChatGPT 等）辅助编程。AI 主要用于：
- 生成代码框架和模板
- 辅助理解和解决特定技术问题（Qt Graphics View、寻路算法等）
- 代码审查和调试建议

所有代码逻辑均由人工设计和审核。详见 AI 使用文档。

---

## License

MIT License — 详见 `LICENSE` 文件。
