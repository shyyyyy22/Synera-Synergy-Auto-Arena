[TOC]
# Synera: Synergy Auto-Arena

> C++ 课程项目 | 自走棋风格 PvE 自动战斗游戏 | Qt 6 + CMake

---

## 1. 项目基本信息

### 1.1 项目名称

Synera: Synergy Auto-Arena

### 1.2 开发环境

| 项目 | 内容 |
|------|------|
| 编程语言 | C++17 |
| GUI 框架 | Qt 6.11.0 |
| 构建工具 | CMake 3.16+ |
| 编译器 | MinGW-w64 64-bit |
| 操作系统 | Windows 11 |

### 1.3 完成程度(详见5)

| 阶段 | 完成程度 |
|------ | ------ |
| 阶段一 | 全部完成 |
| 阶段二 | 全部完成 |
| 阶段三 | 全部完成 |
| 阶段四 | 全部完成 |
---

## 2. 构建与运行方式

### 2.1 构建步骤

```bash
cd Synera_Synergy_Auto_Arena
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

也可直接用 **Qt Creator** 打开 `CMakeLists.txt`，选择 MinGW 64-bit Kit，点击运行。

### 2.2 运行方式

```bash
./build/Synera_Synergy_Auto_Arena.exe
```
---

## 3. 项目结构

```
Synera_Synergy_Auto_Arena/
├── CMakeLists.txt              # CMake 项目配置
├── README.md                   # 本文档
├── resources.qrc               # Qt 资源文件
├── app.rc                      # Windows 应用图标资源
├── assets/
│   ├── logo.ico / logo.png     # 应用图标
│   └── units/                  # 20 个单位 PNG 贴图（大部分32×32）
├── include/
│   ├── core/
│   │   ├── Board.h             # 棋盘类（8×8 六边形格 + 备战区）
│   │   └── Game.h              # 游戏主控类（回合/战斗/商店/羁绊/拖拽）
│   ├── entity/
│   │   ├── Unit.h              # 单位基类（属性/状态机/技能虚函数）
│   │   ├── Heroes.h            # 15 个英雄子类 + 5 个 Boss 类
│   │   └── Player.h            # 玩家数据（HP/金币/等级/祝福）
│   └── gui/
│       ├── GameWindow.h        # 主窗口（顶层布局/状态栏/商店/羁绊）
│       ├── GridItem.h          # 六边形/方形格子渲染
│       ├── UnitItem.h          # 单位渲染（贴图/血条/法力条/拖拽）
│       ├── InfoPanel.h         # 右侧信息面板（属性/买卖/羁绊描述）
│       ├── EquipmentItem.h     # 装备图标 + 拖拽交互
│       ├── EquipmentSlotItem.h # 装备槽背景
│       ├── PauseMenuPanel.h    # 暂停菜单（继续/保存）
│       ├── SettlementPanel.h   # 回合结算面板（胜负/奖励）
│       └── StartMenuWidget.h   # 主菜单（新游戏/读档）
└── src/
    ├── main.cpp                # 程序入口
    ├── core/
    │   ├── Board.cpp           # 棋盘实现（增删移查/邻接/范围/寻路）
    │   └── Game.cpp            # 游戏主控实现（全部游戏逻辑）
    ├── entity/
    │   ├── Unit.cpp            # 单位实现（状态机/寻路/法力/战斗）
    │   ├── Heroes.cpp          # 15 个英雄 + 5 个 Boss 技能实现
    │   └── Player.cpp          # 玩家数据实现
    └── gui/
        ├── GameWindow.cpp      # 主窗口实现
        ├── GridItem.cpp        # 格子渲染
        ├── UnitItem.cpp        # 单位渲染
        ├── InfoPanel.cpp       # 信息面板
        ├── EquipmentItem.cpp   # 装备图标
        ├── EquipmentSlotItem.cpp
        ├── PauseMenuPanel.cpp
        ├── SettlementPanel.cpp
        └── StartMenuWidget.cpp
```

### 3.1 `core/` 模块说明

| 文件 | 职责 |
|------|------|
| `Board` | 棋盘数据结构，管理格子增删移查、六边形邻接计算、范围格计算、敌我半场判断 |
| `Game` | 游戏主控，统筹回合流程、商店、羁绊、装备、拖拽、存档等所有子系统 |

### 3.2 `entity/` 模块说明

| 文件 | 职责 |
|------|------|
| `Unit` | 单位基类，定义 HP/ATK/Range/Mana 属性、FSM 状态机、寻路、技能虚函数 |
| `Heroes` | 15 个英雄子类（Sidon/Luna/Noah 等）+ 5 个 Boss 类（Waterblight/Windblight/Thunderblight/Fireblight Ganon + Ganondorf），各自实现 `castSkill()` |
| `Player` | 玩家数据，包括 HP/金币/等级/经验/人口上限/关卡进度/神兽祝福 |

### 3.3 `gui/` 模块说明

| 文件 | 职责 |
|------|------|
| `GameWindow` | 顶层主窗口，包含状态栏、棋盘视图、商店栏、羁绊侧边栏、结算面板、暂停菜单、开始界面 |
| `GridItem` | 棋盘和备战区格子的图形项，支持六边形/方形两种形状，hover/拖拽高亮 |
| `UnitItem` | 单位图形项，渲染英雄贴图、血条、法力条、星级，支持拖拽交互和浮动伤害数字 |
| `InfoPanel` | 右侧信息面板，显示选中单位的属性、技能描述，提供购买/出售按钮 |
| `EquipmentItem` | 装备图标，支持拖拽到单位上装配 |
| `EquipmentSlotItem` | 装备槽背景图形项 |
| `PauseMenuPanel` | 暂停浮层面板（继续游戏 / 保存游戏） |
| `SettlementPanel` | 回合结算浮层面板（胜/负结果 + 下一关按钮） |
| `StartMenuWidget` | 开始界面（新游戏 / 读取存档 / 退出） |

### 3.4 资源文件说明

| 资源 | 路径 | 说明 |
|------|------|------|
| 英雄贴图 | `assets/units/*.png` | 20 个 PNG 大部分32×32 像素，包含 15 英雄 + 5 Boss |
| 应用图标 | `assets/logo.ico` / `logo.png` | 窗口图标和任务栏图标 |
| Qt 资源 | `resources.qrc` | 将 assets 和 app.rc 打包进 Qt 资源系统 |

---

## 4. 核心类设计

### 4.1 `Game`

#### 职责

游戏主控制器，统筹管理所有子系统：回合流程、商店、羁绊计算、装备拖拽、存档读档、敌人产生。

#### 主要成员变量

| 变量 | 类型 | 说明 |
|------|------|------|
| `m_board` | `Board` | 8×8 主棋盘 |
| `m_bench` | `Board` | 1×8 备战区 |
| `m_player` | `Player*` | 玩家数据 |
| `m_units` | `std::vector<Unit*>` | 所有存活单位（含敌方） |
| `m_gridItems` / `m_benchItems` | `std::vector<GridItem*>` | 棋盘/备战区格子控件 |
| `m_unitItems` | `std::vector<UnitItem*>` | 单位控件 |
| `m_unitItemById` | `std::unordered_map<int, UnitItem*>` | 单位 ID → 控件映射 |
| `m_equipmentByIndex` | `std::unordered_map<int, EquipmentItem*>` | 装备槽索引 → 装备控件 |
| `m_timer` | `QTimer*` | 游戏主循环（60fps） |
| `m_phase` | `GamePhase` | 当前阶段（Prep / Combat / Resolve） |
| `m_heroPools` | `std::vector<QString>` | 英雄池（15 个） |
| `m_raceCount` / `m_professionCount` | `std::map<Race/Profession, int>` | 羁绊计数 |

#### 主要成员函数

| 函数 | 说明 |
|------|------|
| `startNewGame()` | 初始化新游戏 |
| `gameTick()` | 主循环回调（60fps），驱动所有单位 update + 胜负判定 |
| `buildScene()` | 构建 QGraphicsScene（棋盘/备战区/单位/装备槽） |
| `syncFromBoardAndBench()` | 同步数据模型到 GUI |
| `rollShop()` | 随机刷新 5 个商店英雄 |
| `buyHero()` / `sellHero()` | 购买/出售英雄 |
| `buyXp()` | 购买经验（4 金币 → 4 XP） |
| `upUnitStar()` | 3 合 1 升星 |
| `calculateSynergies()` | 计算羁绊并应用 Buff |
| `applySynergyBuffs()` | 对场上单位施加羁绊/祝福 Buff |
| `generateEnemy()` | 根据当前关卡产生敌方阵容（共 17 关） |
| `generateRandomEquip()` | 战斗胜利后概率掉落装备 |
| `saveGame()` / `LoadGame()` | 存档/读档（文本序列化） |

### 4.2 `Board`

#### 职责

棋盘数据结构，管理 8×8 六边形格子和 1×8 备战区格子。

#### 主要成员变量

| 变量 | 类型 | 说明 |
|------|------|------|
| `m_rows` / `m_cols` | `int` | 行数/列数（8×8） |
| `m_cells` | `std::vector<Unit*>` | 格子数组（大小 rows×cols） |
| `m_isBoard` | `bool` | true=棋盘，false=备战区 |

#### 主要成员函数

| 函数 | 说明 |
|------|------|
| `addUnit()` / `removeUnit()` / `moveUnit()` | 单位增删移 |
| `getUnitAt()` / `hasUnitAt()` | 查询/判断某格是否有单位 |
| `getNeighborGrid()` | 获取六边形邻接格（6 个方向） |
| `getRangeGrid()` | 获取指定距离内的所有格子(用于显示攻击范围) |
| `isValidPosition()` | 判断坐标是否合法 |
| `isPlayerHalf()` | 判断是否在己方半场（行号 ≥ 4） |
| `clear()` | 清空棋盘 |

### 4.3 `Unit`

#### 职责

单位基类，封装属性、状态机、寻路、法力值和技能虚接口。

#### 主要属性

| 属性 | 类型 | 说明 |
|------|------|------|
| `m_hp` / `m_maxHp` | `int` | 当前/最大生命值 |
| `m_atk` | `int` | 攻击力 |
| `m_range` | `int` | 攻击距离（格数） |
| `m_mana` / `m_maxMana` | `int` | 当前/最大法力值 |
| `m_name` | `QString` | 单位名称 |
| `m_pos` | `QPoint` | 棋盘坐标 |
| `m_owner` | `Owner` | 阵营（PlayerCtrl / EnemyCtrl） |
| `m_race` | `Race` | 种族（Zora/Rito/Gerudo/Goron/Hyrulean/Boss） |
| `m_profession` | `Profession` | 职业（Warrior/Archer/Mage/Guardian/Assassin） |
| `m_star` | `int` | 星级（1/2/3） |
| `m_state` | `State` | 状态（Idle/Moving/Attacking/Casting/Dead） |
| `m_atkCoolDown` | `int` | 攻击冷却（帧） |
| `m_moveCoolDown` | `int` | 移动冷却（帧） |
| `m_equipment` | `Equipment` | 携带装备（None/Sword/Mail/Gloves/Crystal） |

#### 状态机设计

```
                  ┌────────────────────────────────────────┐
                  │                                        |(敌方单位死亡)
                  ▼                                        |
                 Idle ──(寻敌)──→ Moving ──(进入射程)──→ Attacking
                  |                                        |  
                  │                                        |  (法力满)
                  │     ┌──────────────────────────────────┘
                  │     ▼
                  │  Casting ──(施放完毕)──→ Idle
                  │
                  └──(任意状态 HP=0)──→ Dead
```

- **Idle**: 按索敌规则寻找敌方单位，找到则进入 Moving
- **Moving**: BFS 寻路向目标移动，进入射程后进入 Attacking
- **Attacking**: 冷却完毕后普攻（ATK×1.0），魔力+10/次，满则进入 Casting
- **Casting**: 触发 `castSkill()` 技能，法力归零后回到 Idle
- **Dead**: 从棋盘移除，发出 `isDead` 信号

### 4.4 英雄子类

#### 继承关系

```
Unit (抽象基类)
├── Sidon     (卓拉·守卫)     ├── Revali    (利特·射手)
├── Luna      (卓拉·法师)     ├── Kashi     (利特·战士)
├── Noah      (卓拉·战士)     ├── Evan      (利特·法师)
├── Ur        (格鲁德·战士)   ├── Daruk     (鼓隆·守卫)
├── Naji      (格鲁德·法师)   ├── Goma      (鼓隆·战士)
├── Shika     (格鲁德·刺客)   ├── Mag       (鼓隆·法师)
├── Leo       (海拉鲁·战士)   ├── Alan      (海拉鲁·守卫)
└── Syndra    (海拉鲁·法师)
├── WaterblightGanon   (Boss·水)
├── WindblightGanon    (Boss·风)
├── ThunderblightGanon (Boss·雷)
├── FireblightGanon    (Boss·火)
└── Ganondorf          (Boss·最终，三阶段变身)
```

#### 多态实现方式

基类 `Unit` 声明纯虚函数 `castSkill()`，每个英雄子类重写实现各自的技能效果。`updateUnit()` 和 `takeDamage()` 在基类提供默认实现，特定英雄可选择性重写（如 Sidon 的伤害减免、Ganondorf 的多阶段变身）。

Boss 单位具备二阶段/三阶段变身。

### 4.5 `Player`

#### 职责

管理玩家状态：生命值、金币、等级、经验、人口上限、关卡进度、神兽祝福。

#### 玩家状态数据

| 属性 | 类型 | 初始值 | 说明 |
|------|------|--------|------|
| `m_hp` | `int` | 100 | 玩家生命值（归零则游戏结束） |
| `m_gold` | `int` | 10 | 金币（购买英雄/刷新/经验） |
| `m_level` | `unsigned short` | 1 | 玩家等级（上限 8） |
| `m_maxUnit` | `unsigned short` | 3 | 人口上限 |
| `m_xp` / `m_maxXP` | `unsigned short` | 0/2 | 当前经验/升级所需经验 |
| `m_majorStage` / `m_minorStage` | `unsigned short` | 1/1 | 大关/小关 |
| `m_waterBless` | `bool` | false | 水神兽祝福（+60 HP） |
| `m_windBless` | `bool` | false | 风神兽祝福（+10% 攻速） |
| `m_thunderBless` | `bool` | false | 雷神兽祝福（-10 法力上限） |
| `m_fireBless` | `bool` | false | 火神兽祝福（+10 ATK） |

### 4.6 GUI 相关类

#### `GameWindow`

顶层主窗口，继承 `QMainWindow`。使用 `QStackedWidget` 管理开始界面/游戏主界面切换。包含：
- 顶部状态栏（HP/金币/等级/人口/关卡/神兽祝福）
- 左侧羁绊侧边栏（动态更新激活羁绊列表）
- 中央 QGraphicsView（棋盘渲染）
- 右侧 InfoPanel（单位信息/买卖按钮）
- 折叠商店栏（5 个英雄卡片 + 刷新/升级按钮）
- 底部控制栏 (战斗开始、游戏暂停、商店按钮)
- 浮层：结算面板 / 暂停菜单

#### `GridItem`

继承 `QGraphicsObject`，支持六边形和方形两种形状。根据行号奇偶偏移实现蜂窝排列。支持 hover 高亮、可拖放高亮、攻击范围预览。

#### `UnitItem`

继承 `QGraphicsObject`，渲染英雄贴图、HP 血条（绿色）、法力条（蓝色）、星级标记、装备图标。支持鼠标拖拽交互、点击选中、受伤浮动数字、平滑移动动画（`QPropertyAnimation`）。

#### `InfoPanel`

继承 `QWidget`，显示选中单位的详细属性（名称/HP/ATK/Range/Mana/星级/种族/职业/技能描述）。提供购买/出售按钮。

#### 其他面板/组件

| 组件 | 说明 |
|------|------|
| `StartMenuWidget` | 开始界面，logo + 新游戏/读档/退出按钮 |
| `SettlementPanel` | 回合结算浮层，显示胜负结果 + 奖励 + 下一关按钮 |
| `PauseMenuPanel` | 暂停浮层，继续游戏/保存游戏按钮 |
| `EquipmentItem` | 装备图标（剑/铠/手套/水晶），支持拖拽到单位 |

---

## 5. 游戏规则与功能实现

### 5.1 第一阶段：基础系统

#### 5.1.1 棋盘系统

8×8 六边形蜂窝棋盘，奇偶行交错偏移。上半 4 行为敌方区域，下半 4 行为己方区域。

#### 5.1.2 备战区系统

棋盘下方 1×8 矩形格子作为备战区，用于存放未上场英雄。

#### 5.1.3 单位系统

`Unit` 类封装完整属性：HP / ATK / Range / Max Mana / Mana / 种族 / 职业 / 星级 / 状态 / 装备。

#### 5.1.4 玩家与敌方区分

`Owner` 枚举区分 `PlayerCtrl`（己方）和 `EnemyCtrl`（敌方）。`Board::isPlayerHalf()` 确保己方单位只能放置在己方半场。

#### 5.1.5 GUI 显示

棋盘六边形/备战区方形格子渲染、单位贴图 + 血条 + 法力条、信息面板属性展示。

#### 5.1.6 第一阶段完成情况

| 要求 | 是否完成 | 说明 |
|------|----------|------|
| M×N 棋盘 | ✅ | 8×8，`Board.h` |
| 备战区 | ✅ | 1×8，`Board m_bench` |
| Unit 类 | ✅ | HP/ATK/Range/MaxMana/Mana，`Unit.h` |
| owner 区分敌我 | ✅ | `enum Owner`，Board::isPlayerHalf() |
| traits / 种族职业信息 | ✅ | 5 种族 + 5 职业枚举，`Unit.h` |
| 敌人生成 | ✅ | 17 关预设阵容，`generateEnemy()` |
| GUI 显示棋盘、备战区、单位、属性面板 | ✅ | 9 个 GUI 类 |

---

### 5.2 第二阶段：自动战斗系统

#### 5.2.1 游戏阶段循环

`GamePhase` 枚举驱动三阶段循环：**Prep**（布阵/购物）→ **Combat**（自动战斗，60fps tick）→ **Resolve**（结算胜负/奖励/扣血）→ 下一关 Prep。

#### 5.2.2 单位状态机

五状态 FSM，详见 4.3 节状态机图。在 `Unit::updateUnit()` 中每帧调度：
- `handleIdle()` — 遍历敌方找最近单位
- `handleMoving()` — BFS 寻路每步移动一格
- `handleAttking()` — 冷却递减，归零时普攻（ATK 伤害 + 法力+10）
- `handleCasting()` — 冷却递减，归零时调用 `castSkill()`
- Dead — 从棋盘移除

#### 5.2.3 寻敌逻辑

`handleIdle()` 遍历 `allUnits`，按照索敌规则寻找目标。

#### 5.2.4 移动与寻路算法

`breadFirstSearch()` 实现 BFS 寻路，使用六边形邻接（6 方向），考虑格子占用情况。目标格为空时向目标一格移动，目标被占时尝试相邻可达格。

#### 5.2.5 攻击逻辑

`handleAttking()` 冷却倒计时 → 0 时对目标调用 `takeDamage(m_atk)`。若触发战士羁绊（HP<50% 时伤害×1.3）或射手羁绊（每 3 次攻击 ×1.5）则加成。

#### 5.2.6 法力值与技能释放

- 起始法力 0，每次普攻 +10
- 法力达到 Max Mana 时自动切换到 Casting 状态
- 30 帧技能前摇后调用 `castSkill()`
- 施放后法力归零，若触发法师羁绊则对目标附加 30 帧眩晕

#### 5.2.7 死亡与胜负判定

`gameTick()` 统计双方存活数，一方为 0 则进入 Resolve：
- 玩家胜利 → 金币+4，经验+3，概率获得装备
- 玩家失败 → 根据敌方存活数扣血（5~20），金币+2，经验+1

#### 5.2.8 敌方随回合增强

`generateEnemy()` 根据 `(majorStage-1)*4 + minorStage` 计算当前回合（1~17），敌方属性按 `1.0 + (stage-1)*0.15 + (minor-1)*0.03` 比例增强。

#### 5.2.9 第二阶段完成情况

| 要求 | 是否完成 | 说明 |
|------|----------|------|
| 准备/战斗/结算循环 | ✅ | `GamePhase::Prep/Combat/Resolve` |
| 敌人生成并随轮次增强 | ✅ | 17 关预设 + scale 成长 |
| FSM 状态机 | ✅ | Idle→Moving→Attacking→Casting→Dead |
| 寻敌与移动 | ✅ | BFS 寻路，六边形邻接 |
| 普通攻击 | ✅ | ATK 伤害 + 法力+10 + 羁绊加成 |
| 技能效果，至少 3-5 种 | ✅ | 15 种英雄技能 + Boss 多阶段 |
| 死亡与胜负判定 | ✅ | gameTick() 统计 + 扣血/奖励 |

---

### 5.3 第三阶段：策略系统

#### 5.3.1 商店系统

英雄池 15 个，`rollShop()` 随机选 5 个显示。点击商店卡片预览属性，点击购买按钮花费 3 金币。

#### 5.3.2 金币系统

初始 10 金币。购买英雄 -3，刷新商店 -2，购买经验 -4，出售英雄获得 `2*star² - 3*star + 3` 金币。

#### 5.3.3 经验与等级系统

`buyXp()` 4 金币换取 4 XP。XP 达到 `m_maxXP` 时自动升级（上限 Lv.8），`m_maxXP` 随等级递增。

#### 5.3.4 人口上限系统

`m_maxUnit` 随等级增加，棋盘上己方单位数不能超过人口上限。

#### 5.3.5 羁绊系统

##### 已实现羁绊类型

5 种族 + 5 职业，共 10 种羁绊。

##### 羁绊触发条件

| 羁绊类型 | 第一阈值 | 第二阈值 |
|----------|----------|----------|
| 种族 | 2 单位 | 4 单位 |
| 职业 | 2 单位 | 3 单位 |

Boss 种族不参与羁绊。

##### 羁绊效果

| 羁绊 | 2 单位 | 4 单位 / 3 单位 |
|------|--------|-----------------|
| 海拉鲁 | 全体生命 +150 | 全体生命 +300 |
| 卓拉 | 开局 +20 法力 | 开局 +40 法力 |
| 鼓隆 | 全体最大生命 +10% | +20% |
| 格鲁德 | 全体攻击力 +10 | +20 |
| 利特 | 全体攻速 +10% | +20% |
| 战士 | 全体攻击力 +5 | 生命 <50% 时伤害 +30% |
| 射手 | 攻击距离 +1 | 每 3 次攻击 150% 伤害 |
| 法师 | 最大法力 -20 | 技能命中眩晕目标 |
| 刺客 | 攻速 +15% | 首次攻击 200% 伤害 |
| 守卫 | 全体生命 +100 | 开局伤害免疫一次 |

#### 5.3.6 升星系统

3 个同名同星英雄自动合成 1 个高星英雄（1★→2★→3★）。升星后保留装备、放回原位置。多余装备返还装备槽或折现金币。

#### 5.3.7 装备系统

##### 装备种类

| 装备 | 效果 |
|------|------|
| Sword（剑） | ATK +15 |
| Mail（铠甲） | Max HP +150 |
| Gloves（手套） | 攻速 +20% |
| Crystal（水晶） | 最大法力 -20(根据平衡调整) |

##### 装备效果

装备属性直接叠加到单位基础属性上，`restoreOriAtt()` 时保留装备加成。

##### 装备获取方式

每场战斗胜利后 50% 概率获得随机装备。初始装备槽 4 个，上限 4 个。

##### 装备限制

每单位最多携带 1 件装备。已装备单位不能再次装备，需出售后装备自动返回槽位。

#### 5.3.8 存档与读档系统

##### 保存内容

玩家状态（HP/金币/等级/经验/关卡/祝福）+ 全部己方单位（名称/星级/位置/是否在棋盘/装备）+ 装备槽状态。文本格式，每项一行。

##### 读取流程

`LoadGame()` 读取 `save.txt`，重建玩家状态、创建单位并放置、重建装备槽，调用 `generateEnemy()` 生成当前关卡敌人。

#### 5.3.9 GUI 信息展示

- 顶部状态栏：HP / 金币 / 等级 / 人口 / 关卡 / 神兽祝福（彩色激活标记 + Tooltip 效果）
- 右侧面板：选中单位全属性 + 技能描述 + 购买/出售按钮
- 左侧羁绊栏：动态显示当前激活羁绊名称、进度（已激活/未激活高亮）、效果描述
- 底部商店栏：5 个英雄卡片 + 升级/刷新按钮
- 装备槽：4 个图标，拖拽到单位上装配

#### 5.3.10 第三阶段完成情况

| 要求 | 是否完成 | 说明 |
|------|----------|------|
| 5 个商店位 | ✅ | `rollShop()` 生成 5 英雄，`m_shopSlots` 5 按钮 |
| 金币系统 | ✅ | 初始 10，买/卖/刷新/失败均有金币变动 |
| 经验与等级系统 | ✅ | Lv.1~8，buyXp() 4 金 → 4 XP |
| 人口限制 | ✅ | `m_maxUnit`，拖拽时检查 |
| 4-6 种职业/种族羁绊 | ✅ | 5 种族 + 5 职业 = 10 种羁绊 |
| 至少 2 类阈值羁绊 | ✅ | 种族 2/4、职业 2/3 |
| 3 合 1 升星 | ✅ | `upUnitStar()` |
| 至少 4 种装备 | ✅ | Sword/Mail/Gloves/Crystal |
| 装备携带限制 | ✅ | 每单位 1 件 |
| 存档/读档 | ✅ | save.txt 文本序列化 |
| GUI 完整展示 | ✅ | 状态栏/羁绊栏/信息面板/商店栏 |

---

### 5.4 第四阶段：扩展功能

#### 5.4.1 扩展功能选择

- 神兽祝福系统
- 多阶段 Boss（水/风/雷/火咒盖侬 + 灾厄盖侬 3 阶段）
- 英雄贴图 + 自定义绘制（血条/法力条/星级/装备图标/浮动伤害文字）
- 开始界面 / 暂停菜单

#### 5.4.2 神兽祝福系统设计

每通过一个大区域（第 4/8/12/16 关击败区域 Boss），获得对应神兽的永久祝福。祝福在 `applySynergyBuffs()` 中与羁绊叠加计算。

| 神兽 | 激活时机 | 效果 |
|------|----------|------|
| 瓦·露塔（水） | 击败水咒盖侬 | 全体生命 +60 |
| 瓦·梅德（风） | 击败风咒盖侬 | 全体攻速 +10% |
| 瓦·娜波力斯（雷） | 击败雷咒盖侬 | 全体最大法力 -10（至少 30） |
| 瓦·鲁达尼亚（火） | 击败火咒盖侬 | 全体攻击力 +10 |

#### 5.4.3 多阶段 Boss 设计

| Boss | 阶段数 | 触发条件 |
|------|--------|----------|
| 水/风/雷/火咒盖侬 | 2 阶段 | HP 首次降至 50% 时改变攻击机制 + 属性提升 |
| 灾厄盖侬（最终） | 3 阶段 | HP 降至70%和30%时改变机制 + 属性提升 |

#### 5.4.4 扩展功能 GUI 展示

- 神兽祝福在顶部 `m_pBlessLabel` 显示，已激活用彩色高亮，悬停显示详细效果
- Boss 阶段变身通过属性突变体现
- 暂停菜单浮层覆盖游戏画面

#### 5.4.5 第四阶段完成情况

| 要求 | 是否完成 | 说明 |
|------|----------|------|
| 至少 1 个扩展功能 | ✅ | 神兽祝福、多阶段 Boss、贴图 |
| 功能可实际运行 | ✅ | 全部集成在 gameTick/applySynergyBuffs 中 |
| 有 GUI 或可视化展示 | ✅ | 祝福标签彩色高亮 + Tooltip |
| README 中有说明 | ✅ | 本文档 |

---

## 6. 关键算法说明

### 6.1 棋盘坐标与格子管理

棋盘坐标系 `QPoint(col, row)`。格子存储在 `std::vector<Unit*>` 中，使用 `row * COLS + col` 一维索引。备战区使用 `pos.x()` 直接索引。

### 6.2 六边形邻接计算

```cpp
int dir[6][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,1}};
// 偶数行时调整对角方向
if (r % 2 == 0) {
    dir[4] = {-1,-1};
    dir[5] = {1,-1};
}
```

六边形蜂窝排列：每行 8 格，奇偶行偏移半个格宽，形成交错效果。

### 6.3 BFS 寻路算法

`breadFirstSearch()` 使用 `std::queue` 实现 BFS：
1. 从当前位置出发，六方向扩展
2. 优先空白格，被占格跳过
3. 返回从起点到目标的最短路径
4. `handleMoving()` 每帧沿路径移动一格

### 6.4 单位寻敌策略

`handleIdle()` 遍历全体敌方单位，按照索敌规则层层筛选敌方作为目标。

### 6.5 战斗状态机

FSM 的核心是 `updateUnit()` 中的 switch-case 调度 + 各 handler 的状态转移。转移条件基于冷却倒计时和目标状态变化。

### 6.6 羁绊 Buff 计算

```cpp
calculateSynergies():
  1. 统计场上己方各单位种族/职业数量
  2. 调用applySynergyBuffs()
applySynergyBuffs():
  1. 根据阈值得到 Buff 数值
  2. 对每个单位：先 restoreOriAtt() → 叠加祝福 → 叠加种族羁绊 → 叠加职业羁绊
```

### 6.7 升星合成逻辑

```cpp
upUnitStar(name, star):
  1. 遍历己方所有单位，找到 3 个同名同星单位
  2. 收集装备，移除旧单位
  3. createHeroforPreview(name, star+1) 创建高星单位
  4. 放回第一个旧单位的位置
  5. 多出的装备返回装备槽或折现金币
```

### 6.8 存档序列化逻辑

```cpp
saveGame():
  先写玩家状态（逐行）
  → 计算己方单位数量并写入
  → 遍历己方单位：名称/星级/位置/是否在棋盘/装备
  → 写入 4 个装备槽状态
```

读取时按相同顺序逐行/逐字段解析，调用对应 setter 和 addUnit。

---

## 7. 操作说明

| 操作 | 方式 |
|------|------|
| 开始游戏 | 主菜单点击"开始新游戏" |
| 读档 | 主菜单点击"读取存档" |
| 预览英雄 | 点击商店栏英雄卡片 |
| 购买英雄 | 预览后点击右侧面板"购买"按钮 |
| 刷新商店 | 点击"刷新商店"按钮（-2 金币） |
| 购买经验 | 点击"增加经验"按钮（-4 金币） |
| 上阵/下阵/换位 | 拖拽单位到目标格子 |
| 装备单位 | 拖拽装备图标到己方单位上 |
| 查看属性 | 点击棋盘/备战区中的单位 |
| 出售单位 | 选中单位后点击右侧"出售"按钮 |
| 开始战斗 | 点击"开始对战"按钮 |
| 暂停游戏 | 点击右下角"暂停游戏"按钮 |
| 保存游戏 | 暂停菜单中点击"保存游戏" |
| 下一关 | 结算面板点击"下一关" |
| 退出游戏 | 开始界面/结算面板点击"退出" |

---

## 8. 面向对象设计说明

### 8.1 封装

所有类成员变量均为 private 或 protected(除神兽祝福状态)，通过 getter/setter 访问。`Unit` 的状态机内部逻辑封装在 handler 函数中，外部仅通过 `updateUnit()` 驱动。

### 8.2 继承

`Unit` 抽象基类 → 15 个英雄子类 + 5 个 Boss 子类。`QGraphicsObject` → `GridItem` / `UnitItem` / `EquipmentItem`。`QWidget` → `InfoPanel` / `GameWindow` / `SettlementPanel` 等。

### 8.3 多态

- `virtual void castSkill() = 0` 纯虚函数，每个英雄子类重写
- `virtual void updateUnit()` / `virtual void takeDamage()` 可选重写（Boss 的变身/减伤机制、特定英雄的技能）
- `virtual void setMoveCoolDown()` / `setAtkCoolDown()` 可选重写（最终 Boss 免疫控制）

### 8.4 STL 使用

| 容器 | 使用场景 |
|------|----------|
| `std::vector` | 单位列表、GUI 控件列表、英雄池、商店列表、浮动文字 |
| `std::map` | 种族/职业羁绊计数 |
| `std::unordered_map` | 单位 ID → 控件映射、装备索引映射 |
| `std::unique_ptr` | `createHeroforPreview()` 返回值 |
| `std::queue` | BFS 寻路 |
| `QSet` | 攻击范围格子去重 |

### 8.5 Qt 信号槽设计

Model（Game/Unit）与 View（GUI 控件）通过 signals/slots 解耦：

| 信号 | 发出者 | 接收者 | 说明 |
|------|--------|--------|------|
| `isDead(Unit*)` | Unit | Game | 单位死亡处理 |
| `infoChanged(Unit*)` | Unit | UnitItem / InfoPanel | 属性变化刷新 |
| `damaged(int)` | Unit | UnitItem | 浮动伤害数字 |
| `unitSelected(Unit*)` | Game | InfoPanel | 单位选中显示 |
| `roundFinishend(bool, int, int)` | Game | GameWindow / SettlementPanel | 回合结算 |
| `gameOver(bool)` | Game | SettlementPanel | 游戏结束 |
| `gameIsCombat(bool)` | Game | InfoPanel | 战斗中禁用买卖 |
| `dragStarted/Moved/Dropped` | UnitItem / EquipmentItem | Game | 拖拽交互 |

### 8.6 异常处理或错误处理

- 棋盘操作（add/remove/move）均检查 null 和坐标合法性，非法操作静默返回
- 存档读取失败时自动开始新游戏（`LoadGame()` 返回 false → `initialize()`）
- 拖拽放下的目标检查：`canApplyDrop()` 和 `canApplyEquipDrop()` 前置校验
- 金币不足时购买操作不生效

---

## 9. AI 使用说明

### 9.1 使用的 AI 工具

- Gemini / DeepSeek

### 9.2 AI 辅助的内容

- 代码框架搭建：Qt Graphics View 框架使用方式、CMake 项目结构
- Qt 技术问题：QGraphicsObject 自定义绘制、拖拽交互实现、信号槽连接方式
- Debug 思路：段错误排查、MOC 编译问题、内存泄漏定位，AI帮助我解决了不少的Bug
- UI 设计与代码编写：布局方案(主要是GUI模块的代码)
- README 结构整理
- 数值与技能设计
- 一些核心模块的思路

### 9.3 AI辅助设计的两个核心模块
#### 1.回合结束的敌人清理
* 主要函数`clearEnemyBeforeRound`
* AI辅助原因：清理单位涉及许多内存的释放，在释放的过程上，遇到了困难，利用AI设计整个过程，防止了内存泄漏等问题。
* 核心逻辑：回合结束时，遍历全体单位，如果是敌方单位，则进行以下过程:
  1. 调用removeUnit(),从棋盘上移除
  2. 视觉层面：把对应的UnitItem从m_scene上移除，再从m_unitItems中移除
  3. 执行delete item,释放内存，再从unitItemByID中移除
  4. delete enemy的Unit* 指针，最后从m_unit中移除
#### 2.基于 QHash 逆向路径重建的六边形网格 BFS 寻路
* 主要函数`breadFirstSearch`
* AI辅助原因：一开始我使用正常的BFS，每探索一步记录路径，这样带来的复杂度很高。
* 核心逻辑：BFS算法：维护一个队列，每次从队列中弹出一个pos,调用getNeighborGrid()获取相邻格子，满足条件(可达，未探索)则在哈希表中记录`parentMap[nxtState] = state`,并加入visited列表和队列。 **改进点**：在找到目标状态时，使用哈希表逆向重建路径，即可完成寻路。

---

## 10. 资源来源与许可证

### 10.1 图片资源来源

英雄贴图来自开源/免费游戏素材网站（Kenney、OpenGameArt、CraftPix 等），格式为 32×32 PNG。所有素材符合 CC0 或 CC-BY 许可证。
logo图标由ai生成。

---

## 11. 总结

### 11.1 项目完成情况

全部四个阶段要求已完成：
- **第一阶段**：8×8 六边形棋盘 + 备战区 + 单位属性/阵营/种族/职业 + GUI
- **第二阶段**：Prep/Combat/Resolve 三阶段循环 + 五状态 FSM + BFS 寻路 + 法力值技能系统 + 17 关 PvE
- **第三阶段**：商店/金币/经验/人口 + 10 种羁绊 + 3 合 1 升星 + 4 种装备 + 存档读档
- **第四阶段**：神兽祝福系统 + 多阶段 Boss + 英雄贴图 + 暂停/开始/结算界面

### 11.2 个人收获

- 深入理解 Qt Graphics View 框架的自定义绘制和交互
- 实践了 FSM 状态机在游戏 AI 中的应用
- 掌握了六边形棋盘坐标系统和 BFS 寻路实现
- 理解了 OOP 继承多态在游戏实体设计中的价值
- 学会使用 CMake 管理 Qt/C++ 项目
- 体验了 AI 辅助编程的高效模式

### 11.3 后续展望

- 添加音效和动画效果提升游戏体验
- 实现 PvP 对战功能
