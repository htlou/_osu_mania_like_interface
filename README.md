# Psu! --osu-mania like music game

北京大学2023春程序设计实习QT大作业

## 编译环境

Qt 6.5.1 with multimedia, qmake

**System Requirement: ** Windows 10, 11; MacOS (greater than 11.0)

## 开发注意事项

**To be deprecated in future versions, just a reminder**

1. .h文件中尽量不要放大段的代码实现，如有，最好不超过5行（之前放在.h文件中的构造函数已经被挪到.cpp文件中了）
2. 命名规则：推荐驼峰命名或者下划线分隔，麻烦尽量写清楚变量的用途（或者辅以恰当的注释）
   1. GlobalVariations里面的全局变量最好大写，以与成员变量区分
3. 重要代码实现的逻辑（特别是signal-slot的关系）麻烦尽量在这个README文档里面及时更新

## 功能介绍

**（之后写报告的时候再完善）**

支持多曲目（和谱面编辑？）的四键音游（或许可以在开头考虑加点剧情和关卡，也可以做成osu!那样自行选择导入曲目的方式）

## 项目模块和设计细节

### 1. 主界面

主界面继承自QMainWindow，代码实现见MyMainWindow类，使用QStackedLayout处理不同界面的切换

界面切换的具体实现方式（这样就不用每次新开一个window）

```c++
QWidget *w = layout->currentWidget(); // layout是QStackedLayout*对象，为MyMainWindow类的成员变量
layout->removeWidget(w); // 删除当前的界面(QGraphicsView*)
w->deleteLater();
QGraphicsView* game_view = initGame(); // initGame()返回游戏主界面的QGraphicsView*指针
layout->addWidget(game_view);
layout->currentWidget()->setFocus(); // 强焦点于(Strong Focus on)当前界面，以保证程序能响应键盘输入
```

#### 主界面和其他界面的相互跳转

**键位设置界面：** ```&settings_button::button_pressed``` (signal) -> ```&settings_scene::return_slot``` (slot)-> ```settings_scene::exit_settings``` (signal)-> ```&MyMainWindow::return_from_settings``` (slot)

