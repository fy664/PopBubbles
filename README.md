# Pop Bubbles - 戳泡泡

一款使用 C++17 和 SFML 开发的 2D 动作游戏。

玩家控制角色使用尖针戳破各种泡泡敌人。尖针始终指向鼠标方向，触碰即破——无需点击！左键戳刺可延伸尖针获得更远射程。随着时间推移难度递增，击杀升级提升血量上限，争取获得最高分数！

## 编译与运行

### 依赖

- **C++17** 编译器 (MSVC 2019+ / GCC 8+ / Clang 7+)
- **CMake** 3.16+
- **SFML** 2.6 (Graphics, Audio, Window, System)
- **nlohmann/json** (单头文件，已包含在 src/)

### 编译 (Windows / MSVC)

```bash
# 使用 VS 2022+ 开发者命令行
build_vs.bat
```

或 VS Code: `Ctrl+Shift+B` 构建, `F5` MSVC 调试启动。

### 编译 (Linux)

```bash
sudo apt install libsfml-dev cmake g++
cmake -B build && cmake --build build
```

### 运行

```bash
./build/Debug/PopBubbles.exe   # Windows
./build/PopBubbles             # Linux
```

## 游戏操作

| 操作 | 按键 |
|------|------|
| 移动 | W / A / S / D |
| 瞄准 | 移动鼠标 |
| 戳刺攻击 | 鼠标左键 |
| 暂停/返回 | ESC |

## 游戏机制

### 尖针攻击
玩家手持尖针从球心伸出，始终指向鼠标方向。**尖针碰到泡泡即造成伤害（被动碰撞，无需点击）**。点击鼠标左键时尖针向前延伸 35px（0.3s），获得更远射程。戳巨型泡泡时双方产生回弹效果。

### 泡泡类型

| 类型 | 颜色 | 半径 | 行为 | 分数 |
|------|------|------|------|------|
| 普通泡泡 | 随机 | 13px | 持续向玩家移动，泡泡间碰撞弹开 | +10 |
| 射手泡泡 | 紫色 | 13px | 保持 180-280px 距离，每 2.5s 发射子弹 | +30 |
| 巨型泡泡 | 橙色 | 26px | 缓慢追踪，140px 内冲刺，2HP，需攻击两次 | +50 |

### 等级系统
- 每击杀 10 个泡泡升一级
- 升级时血量上限 +2（基础 5HP → Lv2=7 → Lv3=9…）
- 升级时血量回满
- 1 秒内连续击杀 5+ 触发 combo 加分

### 难度系统
- 每 **7.5 秒**提升一个难度等级
- 敌人生成间隔从 1.5s 逐渐缩短至 0.25s
- 难度 3+ 出现射手泡泡 (35%)
- 难度 5+ 出现巨型泡泡 (10%)

### 玩家
- 基础 5 点生命值，每级 +2 上限
- 被泡泡/子弹碰撞损失 1 点生命
- 受伤后有 1.5 秒无敌时间（闪烁效果）
- 可被巨型泡泡击退

## 音效

音效接口已预留。添加音效文件的方法：

1. 将 `.wav` 或 `.ogg` 文件放入 `assets/audio/` 目录
2. 在代码中加载：
```cpp
auto& rm = ResourceManager::getInstance();
rm.loadSound("bubble_pop", "assets/audio/pop.wav");
rm.loadSound("player_hurt", "assets/audio/hurt.wav");
```
3. 播放：
```cpp
sf::Sound sound(rm.getSound("bubble_pop"));
sound.play();
```

## 中文支持

程序自动尝试加载系统中文 TrueType 字体（微软雅黑、黑体等）。
如需自定义字体，将 `.ttf` 文件放入 `assets/` 目录并在代码中调用：
```cpp
ResourceManager::getInstance().loadFont("default", "assets/YourFont.ttf");
```

## 排行榜

排行榜数据存储在 `leaderboard.json` 文件中（位于可执行文件目录），自动保留前 10 条记录，按得分降序排列。

## 项目结构

```
PopBubbles/
├── CMakeLists.txt
├── README.md
├── .vscode/              # VS Code 配置
│   ├── tasks.json
│   └── launch.json
├── src/
│   ├── main.cpp          # 程序入口
│   ├── core/             # 核心系统
│   │   ├── Game.*        # 游戏主类 (1280x900窗口, 状态栈)
│   │   ├── State.*       # 状态基类
│   │   ├── ResourceManager.*  # 资源管理器 (字体/纹理/音效)
│   │   ├── Input.*       # 输入管理 (pressed/down/released)
│   │   ├── Button.*      # 按钮组件 (中心原点缩放)
│   │   ├── TextBox.*     # 文本输入框 (UTF-8输入)
│   │   └── Audio.*       # 音效管理 (8通道, 预留接口)
│   ├── entities/         # 游戏实体
│   │   ├── Entity.*      # 实体基类
│   │   ├── Player.*      # 玩家 (被动尖针+等级系统)
│   │   ├── Bubble.*      # 普通泡泡 r=13
│   │   ├── ShooterBubble.*  # 射手泡泡 r=13
│   │   ├── GiantBubble.* # 巨型泡泡 r=26 2HP
│   │   ├── Bullet.*      # 子弹 r=5 + 100发对象池
│   │   └── Collision.h   # 碰撞检测 (线段/圆/矩形)
│   ├── states/           # 游戏状态 (5个)
│   │   ├── MainMenuState.*   # 主菜单 (装饰泡泡背景)
│   │   ├── TutorialState.*   # 4步教程
│   │   ├── GameState.*       # 核心玩法 (连击/浮动文字/粒子)
│   │   ├── GameOverState.*   # 提交成绩
│   │   └── LeaderboardState.* # Top10 金/银/铜
│   └── system/
│       └── Leaderboard.* # JSON读写, 自动排序
├── assets/               # 资源目录
│   └── audio/            # 音效文件 (放.wav/.ogg激活)
├── CLAUDE.md             # AI开发速查
└── build_vs.bat          # 一键编译脚本
```

## 许可

MIT License
