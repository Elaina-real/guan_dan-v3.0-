# 关丹卡牌游戏 (Guan Dan Card Game)

<div align="center">

一款基于 C++ 和 EasyX 图形库开发的四人卡牌游戏，支持人机对战和记牌器功能。

[![Version](https://img.shields.io/badge/version-4.0-blue.svg)](https://github.com/Elaina-real/guan_dan-v3.0-)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)]()

</div>

---

## 项目简介

关丹是一款经典的四人卡牌游戏，采用红蓝两队对战模式。游戏支持 1 个人类玩家与 3 个 AI 玩家对战，拥有完整的图形界面、音效系统和记牌器功能。

### 主要特点

- 🎮 **完整游戏流程**：开局 → 献牌 → 发牌 → 出牌 → 结算
- 🤖 **智能 AI 对手**：3 个 AI 玩家，提供挑战性的游戏体验
- 📊 **实时记牌器**：追踪剩余牌张，辅助策略决策
- 🎨 **精美图形界面**：基于 EasyX 开发的直观界面
- 🎵 **背景音乐与音效**：增强游戏沉浸感
- 📈 **积分系统**：完整的战绩统计和排行榜

---

## 游戏规则

### 基本规则

- **玩家配置**：4 人游戏，分为红蓝两队
  - 红队：2 人（包括人类玩家）
  - 蓝队：2 人（AI 玩家）
- **目标**：率先出完所有手牌的队伍获胜
- **等级系统**：从 2 开始，获胜后等级提升

### 牌型说明

| 牌型 | 描述 | 示例 |
|------|------|------|
| 单张 | 任意一张牌 | A |
| 对子 | 两张相同的牌 | 33 |
| 三张 | 三张相同的牌 | 777 |
| 三带二 | 三张相同 + 一对 | 888 + KK |
| 炸弹 | 四张相同的牌 | 4444 |
| 双王 | 大王 + 小王 | 大王 + 小王 |
| 四王 | 四张王牌 | 大王 + 小王 × 2 |
| 顺子 | 五张及以上连续单牌 | 34567 |
| 同花顺 | 同花色的顺子 | ♣34567 |
| 铁板（同花顺子） | 同花色的顺子 | ♠34567 |
| 连对 | 三对及以上连续对子 | 334455 |

### 特殊规则

- **等级牌**：当前等级的牌可以作为任意牌使用（类似王的功能）
- **献牌机制**：游戏开始前，低等级玩家需要向高等级玩家献牌
- **还牌机制**：收到献牌的玩家需要返还一张牌

---

## 技术栈

### 核心技术

- **编程语言**：C++11
- **图形库**：EasyX
- **开发环境**：Visual Studio 2022
- **平台**：Windows

### 设计模式

- **工厂模式**：`CardGroupFactory` 负责创建不同类型的牌组
- **策略模式**：`HumanPlayer` 和 `AIPlayer` 实现不同的出牌策略
- **组合模式**：`CardGroup` 及其子类形成继承体系
- **观察者模式**：图形界面与游戏逻辑的交互

### C++ 特性

- 智能指针 (`shared_ptr`, `unique_ptr`, `weak_ptr`)
- RAII 惯例
- STL 容器 (`vector`, `unordered_map`, `array`)
- 多态与虚函数

---

## 项目结构

```
guan_dan(v4.0)/
├── guan_dan(v3.0)/              # 源代码目录
│   ├── main.cpp                 # 程序入口
│   ├── card.cpp/h               # 扑克牌类和牌堆
│   ├── cardcounter.cpp/h        # 记牌器
│   ├── cardgroup.cpp/h          # 牌组基类
│   ├── cardgroupfactory.cpp/h   # 牌组工厂
│   ├── game.cpp/h               # 游戏主逻辑
│   ├── graphical.cpp/h          # 图形界面
│   ├── player.cpp/h             # 玩家类（人类/AI）
│   ├── resource/                # 资源文件夹
│   │   ├── *.png                # 卡牌图片
│   │   ├── *.wav                # 音效文件
│   │   └── *.jpg                # 背景图片
│   └── *.vcxproj                # Visual Studio 项目文件
├── .gitignore                   # Git 忽略文件
└── README.md                    # 项目说明文档
```

---

## 安装和运行

### 环境要求

- **操作系统**：Windows 10 或更高版本
- **开发工具**：Visual Studio 2022（推荐）或 Visual Studio 2019
- **EasyX 库**：[EasyX 官网](https://easyx.cn/)

### 安装步骤

1. **克隆项目**
   ```bash
   git clone https://github.com/Elaina-real/guan_dan-v3.0-.git
   cd guan_dan(v4.0)
   ```

2. **安装 EasyX**
   - 访问 [EasyX 官网](https://easyx.cn/)
   - 下载并安装 EasyX 库

3. **打开项目**
   - 使用 Visual Studio 打开 `guan_dan(v3.0).sln`

4. **配置项目**
   - 确保项目配置为 `Release|x64` 或 `Debug|x64`
   - 确保 EasyX 库路径正确

5. **编译运行**
   - 按 `F5` 或点击"开始调试"按钮

### 游戏操作

- **鼠标点击**：选择卡牌
- **出牌按钮**：出选中的牌
- **过牌按钮**：跳过当前回合
- **记牌器**：查看剩余牌张统计

---

## 功能模块

### 1. 游戏核心模块 (Game)

- 管理游戏状态和流程
- 处理玩家轮流出牌
- 判断游戏胜负
- 积分结算系统

### 2. 玩家模块 (Player)

- `HumanPlayer`：人类玩家，通过图形界面交互
- `AIPlayer`：AI 玩家，自动出牌策略
- 玩家属性管理（分数、等级、手牌等）

### 3. 卡牌系统 (Card & CardGroup)

- 扑克牌类：表示单张卡牌
- 牌组类：支持多种牌型识别和比较
- 牌组工厂：自动识别牌型

### 4. 记牌器 (CardCounter)

- 实时统计剩余牌张
- 按花色、点数分类统计
- 排除玩家手牌的统计功能

### 5. 图形界面 (Graphical)

- 卡牌显示与交互
- 游戏信息展示
- 用户输入处理
- 动画效果

---

## 版本历史

### v4.0 (2025-06-18)

- ✨ 大幅优化游戏架构与功能
- 🔧 重构游戏核心逻辑
- 🎨 增强图形界面功能
- 🤖 改进玩家系统和 AI 策略
- 📊 完善记牌器功能
- 📝 代码规范化，提升可维护性

### v2.0 (2025-06-09)

- 🎮 完善游戏核心功能
- 📈 添加积分系统

### v1.0 (2025-05-13)

- 🎉 项目初始化
- 📦 基础框架搭建

---

## 开发者

- **开发者**：Elaina
- **GitHub**：[Elaina-real](https://github.com/Elaina-real)

---

## 许可证

本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

---

## 贡献

欢迎提交 Issue 和 Pull Request！

如果你有任何建议或发现了 Bug，请随时提出。

---

## 致谢

- 感谢 EasyX 团队提供的优秀图形库
- 感谢所有为本项目做出贡献的开发者

---

<div align="center">

**如果这个项目对你有帮助，请给一个 ⭐ Star！**

Made with ❤️ by Elaina

</div>
