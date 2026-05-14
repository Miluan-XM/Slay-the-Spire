# Slay the Spire (C++ Console Edition / 控制台版)

A C++ console roguelike deck-builder inspired by Slay the Spire.  
仿《杀戮尖塔》的 C++ 控制台 Roguelike 卡牌游戏。

---

## 简介 | Introduction

3-floor map exploration + random events + turn-based card battle.  
三层关卡地图探索 + 随机事件 + 回合制卡牌战斗。

Core features / 核心功能:
- **Map System / 地图系统**: 2-option branching choices at each step / 每步 2 选 1 分支路径
- **Battle System / 战斗系统**: Energy-based card play, buff/debuff mechanics / 能量制出牌，Buff/Debuff 机制
- **Random Events / 随机事件**: Treasure, mystery, rest nodes / 宝箱、神秘、篝火事件
- **4 Enemy AI types / 4 种敌人 AI**: Buff-attack, random, turn-triggered, HP-threshold / 强化攻击、随机、回合触发、血量触发

---

## 快速开始 | Quick Start

### 编译 | Build

```bash
cd build
cmake ..
make
```

### 运行 | Run

```bash
./MY_SlayTheSpire
```

### 依赖 | Dependencies

- CMake >= 3.10
- C++17 compiler (g++ / clang++)
- No third-party libraries required / 无需第三方库

---

## 游戏玩法 | Gameplay

### 操作 | Controls

| Key / 按键 | Action / 操作 |
|-------------|---------------|
| `1` `2` | Choose menu option / 选择菜单选项 |
| `0`–`9` | Play card from hand / 出牌 |
| `-1` | End turn / 结束回合 |
| `Ctrl+D` | Exit game / 退出游戏 |

### 地图系统 | Map System

```
Floor 1 / 第 1 层 → [Step 1] → [Step 2] → [Step 3] → Boss
Floor 2 / 第 2 层 → [Step 1] → [Step 2] → [Step 3] → Boss
Floor 3 / 第 3 层 → [Step 1] → [Step 2] → [Step 3] → Boss → Victory!
```

Event probabilities / 事件概率:
| Event / 事件 | Chance / 概率 | Effect / 效果 |
|--------------|---------------|---------------|
| Enemy / 战斗 | 50% | Random enemy fight + card reward / 随机敌人 + 选卡奖励 |
| Treasure / 宝箱 | 15% | Gold + card reward / 金币 + 选卡 |
| Mystery / 神秘 | 15% | Random: heal/gold/damage/card / 随机: 回血/金币/受伤/得卡 |
| Rest / 篝火 | 20% | Heal 30% max HP / 恢复 30% 最大生命 |

### 战斗系统 | Battle System

- **Energy / 能量**: 3 per turn / 每回合 3 点
- **Draw / 抽牌**: 5 cards per turn / 每回合抽 5 张
- **Discard / 弃牌**: Remaining hand discarded at turn end / 回合结束弃手牌
- **Reshuffle / 洗牌**: Discard pile reshuffles into draw pile when empty / 抽牌堆空时自动洗入弃牌堆

Damage formula / 伤害公式:
```
Base + Strength → *0.75 if Weak → *1.5 if Vulnerable → -Block → apply to HP
```

---

## 项目结构 | Project Structure

```
Slay-the-Spire/
├── include/           # Header files / 头文件
│   ├── Game.hpp       # Core data structures & declarations / 核心数据结构与声明
│   ├── Battle.hpp     # Battle system / 战斗系统
│   ├── Effect.hpp     # Card & move effect resolvers / 效果解析器
│   ├── DataLoader.hpp # CSV file loading / 数据文件加载
│   └── Debug.hpp      # Debug print utilities / 调试打印
├── src/               # Source files / 源文件
│   ├── main.cpp       # Main entry & game loop / 主入口与游戏循环
│   ├── Game.cpp       # Core logic / 核心逻辑
│   ├── Battle.cpp     # Battle system implementation / 战斗实现
│   ├── Effect.cpp     # Effect resolvers / 效果实现
│   ├── DataLoader.cpp # Data loading / 数据加载
│   ├── Debug.cpp      # Debug output / 调试输出
│   └── Map.cpp        # Map & event system / 地图与事件
├── Data/              # CSV data files / 数据文件
│   ├── CardData.txt   # Card definitions / 卡牌数据
│   ├── EnemyData.txt  # Enemy blueprints / 敌人蓝图
│   └── MoveData.txt   # Move definitions / 招式数据
├── build/             # Build output / 构建输出
├── CMakeLists.txt
└── readme.md
```

---

## 数据格式 | Data Format

### CardData.txt

```
id,name,CardType,Cost,e1,e2,e3,e4,e5,v1,v2,v3,v4,v5
```

- `CardType`: 1=ATTACK, 2=SKILL, 3=ABILITY, 4=CURSE
- `e1..e5`: Effect type / 效果类型
- `v1..v5`: Effect value / 效果数值

> Example: `1,打击,1,1,1,0,0,0,0,6,0,0,0,0` = "Strike", ATTACK, cost 1, effect ATTACK(1) value 6

### EnemyData.txt

```
id,name,max_health,ai_mode,Critical:N,moveID1..7,{v1:v2:v3}...
```

- `ai_mode`: 1=INTENSIFY_ATTACK, 2=RANDOM, 3=ROUND_TRIGGER, 4=BLOOD
- `moveID1..7`: 7 move slots / 7 个招式槽位
- `{v1:v2:v3}`: Move values (3 ints per slot) / 每个招式的 3 个数值

### MoveData.txt

```
id,name,intention1,intention2,intention3
```

- Start from ID 501 / ID 从 501 开始
- `intention`: 1=ATK, 2=DEF, 3=CARD, 1001+=self buff, 1601=enemy buff, 2001+=target debuff

---

## 免责声明 | Disclaimer

**This is a hobby project.** It is not affiliated with or endorsed by the original *Slay the Spire* developers (Mega Crit Games). All game design concepts are inspired by the original title.  
**纯兴趣爱好项目。** 与原始《杀戮尖塔》开发者 Mega Crit Games 无关。所有游戏设计概念受原作启发。

---

*Made for fun. / 仅为娱乐而作。*
