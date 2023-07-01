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

### 2. 游戏界面

游戏界面继承自QGraphicsScene，代码实现见GameScene类；启动游戏界面后，程序运行的主要步骤如下 (逻辑主要体现在```GameScene::startGame```函数中) ：

1. 从txt文件中读取谱面数据到GameScene的成员变量```QVector<QPair<int, int> > tm[11]```，```tm[i]```是第 i个音轨对应的下落键的```QVector```数组；```tm[i][t].second == -1```代表短键，否则为长键
2. 初始化游戏界面的背景 (包括计分栏、背景图、下落轨道)；
3. 初始化下落键
4. 计时器开始运行
5. 播放音乐

#### 2.1 下落键的实现

FallingKey类为下落键的代码实现，继承自QGraphicsPixmapItem, 同时兼具长键和短键的实现，预留了自定义外观的接口（成员变量stylePath).

FallingKey同时支持长键和短键，通过变量longKey辨别长短键

GameScene中使用keyFallingTimer每隔INTERVAL毫秒判断是否有键下落，使用e_timer记录当前时刻；如果暂停，暂停时间会累加到pauseTime里面（pauseClock记录上一个开始暂停的时刻）；目前时间方面e_timer和clock()两种方式并存，建议之后统一到e_timer，因为clock记录的是cpu时间，跨平台可能有比较多的问题

 与GameScene类的交互如下

1. ```bool isFalling```变量用于判断键是否在下落，方便GameScene的暂停；
2. GameScene类使用```QTimer *keyFallingTimer```判断当前是否有键下落，```QTimer::timeout```连接槽函数```GameScene::timerFallingKey```
3. 键在下落到底部的时候会emit signal: ```FallingKey::endOfFalling (signal) -> GameScene::handleEndOfFalling (slot)```

#### 2.2 目前下落键存在的问题

如果速度调整，和时间序列的判定不匹配

下落键的提前量确定、长键的长度有待商榷（两个关键的变量是INTERVAL和VELOCITY）

键下落到底部会闪退 -- 目测是deleteLater，然后fallingKeys的数据结构里面又没有释放掉造成的; 目前暂时的解决办法是隐藏

暂停的时候，还没有下落的键会叠在一起（这个问题已经解决了，gamescene.cpp的第278行加了pauseTime）

#### 2.3 暂停功能的实现



# 日志部分

## 2023/6/29

彻底重构了FallingKey类（构造需要的参数更少）

完美实现了暂停（暂停时正在下落的键也会暂停，而且不影响还没下落的键；同时音乐也会暂停）

## 2023/6/30

1. 更改了下落键的处理方式：一旦键开始下落，就把键加到队列queueFalling中，并从multimap里面移除；endOfFalling的时候立即delete下落的键，并且通过GameScene的handleEndOfFalling槽函数清理queueFalling队列
2. 完善了暂停界面（不再新建窗口，在原有界面上新增一些元素）
3. 有了初步的选择曲谱界面，修改文件读入存储方式

7.1日希望完成：主菜单界面修改（类似暂停界面）+选择曲谱界面精修；
我明天准备调试下落键的问题，现在有些bug，如果时间多就做一些数字显示

## 2023/7/1

1. 修改了FallingKeys，现在已经可以正常判定
2. 把Selection上下改成了WS，比较符合人类习惯，Enter改成了Space，方便开始。

