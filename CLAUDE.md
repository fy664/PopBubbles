# PopBubbles — AI 开发速查

C++17 + SFML 2.6.2 戳泡泡游戏，MSVC 2026 编译，VS Code 开发。

## 快速开始
```bash
./build_vs.bat                    # 编译 (vcvars64 + cmake + msbuild)
./build/Debug/PopBubbles.exe      # 运行
```
VS Code: `Ctrl+Shift+B` 构建, `F5` 调试 (cppvsdbg)。

## 架构
```
main.cpp → Game::run() 循环:
  Input::update() → handleEvents() → handleInput() → update(dt) → render()

状态栈 (vector<unique_ptr<State>>):
  MainMenu → Tutorial | Game → GameOver → Leaderboard
```
- 状态虚函数调用顺序: `handleEvent` → `handleInput` → `update` → `render`
- `handleInput` **必须被 Game 主循环显式调用**（之前踩坑：忘了调用导致按钮无响应）
- 状态切换时按钮回调可能销毁自己，`handleInput` 里必须 `return` 退出

## 关键文件
| 层 | 文件 | 职责 |
|----|------|------|
| 核心 | `core/Game.*` | 窗口 1280×900, 状态栈, 主循环 |
| 核心 | `core/State.h` | 虚函数: handleEvent/handleInput/update/render |
| 核心 | `core/Input.*` | 键盘鼠标 pressed/down/released 三态 |
| 核心 | `core/Button.*` | origin 在中心, 宽字符串, 三态缩放 |
| 核心 | `core/Audio.*` | 8 通道音效, 预留接口 (需放 .wav 到 assets/audio/) |
| 实体 | `entities/Player.*` | WASD, 尖针从球心伸出 55px, 被动碰撞, Lv 系统 |
| 实体 | `entities/Bubble.*` | 普通 r=13, 射手 r=13, 巨型 r=26 2HP |
| 实体 | `entities/Bullet.*` | r=5, 100 发对象池 |
| 碰撞 | `entities/Collision.h` | lineCircle / circleCircle / rectCircle |
| 状态 | `states/GameState.*` | 生成/碰撞/连击/粒子/浮动文字 |
| 数据 | `system/Leaderboard.*` | JSON Top10, nlohmann/json |

## 红线
- **所有中文 UI 文本必须 `L"..."` 宽字符串**。`std::string` → SFML 按 GBK 解析会乱码
- **`std::stack<unique_ptr>` 不可拷贝**。已改用 `vector<unique_ptr>` + push_back/pop_back
- **按钮点击回调可能销毁当前状态**。handleInput 中必须 break/return
- **CMake 默认不在 PATH**。用 `build_vs.bat` 或 `~/bin/cmake` wrapper
- **字体加载顺序**：simhei.ttf → simfang.ttf → msyh.ttc。SFML 对 .ttc 支持有限

## 实体参数速查
| 实体 | 半径 | 速度 | 备注 |
|------|------|------|------|
| Player | 18 | 350 | HP 基础 5, 每级 +2 |
| Bubble | 13 | 140 | +10 分 |
| Shooter | 13 | 110 | 保持 180-280px, 2.5s 射击, +30 |
| Giant | 26 | 70/200 | 冲刺 140px 内, 1s CD, 2HP, +50 |
| Bullet | 5 | 280 | 4s 寿命 |

## 难度曲线
- 初始生成间隔 1.5s, 每 7.5s 升一级
- 间隔每次 -0.15s (最低 0.25s)
- Lv3+ 出射手泡泡 (35%), Lv5+ 出巨型 (10%)

## 中文渲染
- 源码 UTF-8 + `/utf-8` 编译标志
- `Button` / `TextBox` 内部用 `std::wstring`
- `TextBox::getText()` 通过 `sf::Utf8::fromWide` 转 UTF-8 输出
- 窗口标题传 `L"..."`

## 深入文档
- 完整游戏机制: `README.md`
- 构建说明: `README.md` §编译与运行
- 音效添加: `assets/audio/README.txt`
