# Pop Bubbles - 戳泡泡

一款使用 C++17 和 SFML 开发的 2D 动作游戏。

玩家控制角色使用尖针戳破各种泡泡敌人，随着时间推移难度递增，争取获得最高分数！

## 编译与运行

### 依赖

- **C++17** 编译器 (GCC 8+ / MSVC 2019+ / Clang 7+)
- **CMake** 3.16+
- **SFML** 2.5+ (Graphics, Audio, Window, System)
- **nlohmann/json** (通过 CMake FetchContent 自动下载)

### 安装 SFML (Windows/MinGW)

```bash
# 使用 MSYS2 安装
pacman -S mingw-w64-x86_64-sfml mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
```

### 安装 SFML (Ubuntu/Debian)

```bash
sudo apt install libsfml-dev cmake g++ gdb
```

### 编译

```bash
cd PopBubbles
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

或使用 VS Code：按 `Ctrl+Shift+B` 运行 CMake Build 任务。

### 运行

```bash
./build/PopBubbles.exe   # Windows
./build/PopBubbles       # Linux
```

或按 **F5** 在 GDB 调试器中启动。

## 游戏操作

| 操作 | 按键 |
|------|------|
| 移动 | W / A / S / D |
| 瞄准 | 移动鼠标 |
| 戳刺攻击 | 鼠标左键 |
| 暂停/返回 | ESC |

## 游戏机制

### 尖针攻击
玩家手持一根尖针，尖针始终指向鼠标方向。点击鼠标左键时，尖针向前快速伸出约 30 像素，持续 0.3 秒后收回。尖针碰到泡泡即造成伤害。

### 泡泡类型

| 类型 | 颜色 | 行为 | 分数 |
|------|------|------|------|
| 普通泡泡 | 随机颜色 | 持续向玩家移动 | +10 |
| 射手泡泡 | 紫色 | 保持 200-300px 距离，每 3 秒发射子弹 | +30 |
| 巨型泡泡 | 橙色 | 缓慢移动，近身冲刺，需攻击 2 次 | +50 |

### 难度系统
- 每 15 秒提升一个难度等级
- 敌人生成间隔逐渐缩短
- 难度 3+ 出现射手泡泡
- 难度 5+ 出现巨型泡泡

### 玩家
- 初始 5 点生命值
- 被泡泡碰撞损失 1 点生命
- 受伤后有 1.5 秒无敌时间（闪烁效果）

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
│   │   ├── Game.*        # 游戏主类
│   │   ├── State.*       # 状态基类
│   │   ├── ResourceManager.*  # 资源管理器
│   │   ├── Input.*       # 输入管理
│   │   ├── Button.*      # 按钮组件
│   │   └── TextBox.*     # 文本输入框
│   ├── entities/         # 游戏实体
│   │   ├── Entity.*      # 实体基类
│   │   ├── Player.*      # 玩家
│   │   ├── Bubble.*      # 普通泡泡
│   │   ├── ShooterBubble.*  # 射手泡泡
│   │   ├── GiantBubble.* # 巨型泡泡
│   │   ├── Bullet.*      # 子弹+对象池
│   │   └── Collision.*   # 碰撞检测
│   ├── states/           # 游戏状态
│   │   ├── MainMenuState.*   # 主菜单
│   │   ├── TutorialState.*   # 教程
│   │   ├── GameState.*       # 游戏主状态
│   │   ├── GameOverState.*   # 游戏结束
│   │   └── LeaderboardState.* # 排行榜
│   └── system/
│       └── Leaderboard.* # 排行榜数据
└── assets/               # 资源文件
```

## 许可

MIT License
