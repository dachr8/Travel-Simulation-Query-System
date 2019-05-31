# 旅行模拟查询系统用户使用说明
[toc]
## 一、Docker配置操作步骤
Docker Image: fdt/travel:v1
(https://storage.yiban.bupt.link/f/161e2d15687f4b848281/?dl=1)
启动步骤
- 如果是第一次使用本系统
1. 下载上述 tar 文件
2. 打开 PowerShell 执行 `docker load --input <上述文件路径>`
3. 执行 `docker run -it -p 127.0.0.1:8180:80 --name travel fdt/travel:v1`
4. 打开浏览器访问 [http://localhost:8180](http://localhost:8180)
- 如果不是第一次启动本系统
1. 打开 PowerShell 输入 `docker start -i travel` 指令
2. 打开浏览器访问 [http://localhost:8180](http://localhost:8180)

## 二、主要功能界面介绍及说明
### 界面概况：
- 界面主要分为三部分，左侧为**信息显示模块** (Pointed Traveller Travel Infomation)，显示如下信息：
    - 选择的旅行用户用户名 (Passenger)
    - 当前路程起始地点名 (From)
    - 当前路程重点地点名 (To)
    - 交通工具信息 （Transportation)
    - 当前路程开始时间 (Start Time)
    - 当前路程结束时间 (End Time)
    - 当前路程所需票价 (Cost)
    - 全部旅程结束时间 (Total End Time)
    - 全部旅程总价 (Total Cost)
    - 终极目的地 (Ultimate Destination)
- 右侧为**用户操作模块**，有如下功能：
    - 切换时间流动与静止状态，按钮为灰色为模拟时间静止，绿色为模拟时间流动
    - 模拟时间显示 (Current Time)
    - 模拟旅行的用户名输入 (Passenger)
    - 旅行策略选择 (Travel Strategy)
        - 最少花销 (MIN_COST)
        - 最短时间 (MIN_TIME)
        - 限时最少费用策略 (LIMITED_TIME)
    - 旅行起始地点选项 (From)
    - 旅行终止地点选项 (To)
    - 途径城市（多选） (Pass By Cities)
    - 到达终止地点时间限制（当“限时最少费用策略”被选中时有效，其他策略时为可选项）(Total Time Limit)
    - 旅行开始时间 (Start Time)
        - 注：旅行开始时间不得早于当前模拟时间
- 中间部分为**模拟地图展示模块**
    - 根据用户需求及计算返回结果模拟旅行计划
    - 当鼠标焦点位于代表某旅客的原点上时，左侧栏会同步更新该旅客旅程信息
    ![](https://github.com/dachr8/Travel-Simulation-Query-System/blob/master/document/users-instructions/interface1.png?raw=true)
### 提交旅行计划说明
1. 用户在右侧输入用户名，每一个用户名将唯一对应一位用户，若用户输入重复id，并提交新的计划，将会使之前输入的旅行规划被新计划覆盖，即系统认为用户改变了需求。
2. 用户在输入完用户名后输入旅行需求，包括起点、重点、途经城市、旅行策略以及出发时间。若旅行策略为限时最省的话，则还需输入时间限制。所有输入选项均为下拉菜单式，便于用户进行选择。其中途经城市的选择可以多选，其他选项为单选。
3. 当用户输入完所有请求之后，点击提交按钮，此时在界面上方会根据返回结果提示用户，若显示为绿色的"Ok"（如下图1所示），表示用户所提需求已成功规划路线；若为红色的"Unreachable"（如下图2所示），说明以当前的输入条件并不能成功规划出路径，可能是超过时限或是缺少相应航班等原因导致。
    ![](https://github.com/dachr8/Travel-Simulation-Query-System/blob/master/document/users-instructions/ok.png?raw=true)
    图1 - 请求添加成功
    ![](https://github.com/dachr8/Travel-Simulation-Query-System/blob/master/document/users-instructions/unreachable.png?raw=true)
    图2 - 请求失败，请检查输入值
### 更改旅行计划说明
- 用户若需要更改旅行计划，直接在右侧输入框更改即可。
    注意：
    - 用户名不能更改，否则将会添加新用户。
    - 在模拟过程进行时，用户不能更改模拟行程的起点，起点将默认为此趟飞机或列车的终点，其他需求输入与之前相同。
### 旅行模拟及旅客信息显示说明
- 旅行模拟显示模块在中央，城市节点以大圆圈表示，标有城市名称，城市之间的连线表示城市之间存在通路。城市节点之间互相存在引力和斥力，从而形成一个稳定系统，每个节点可以随意拖动，观感与使用感新颖而别致，可以有效减轻用户在等待模拟过程时的无聊体验。
- 用户以小圆圈表示，标有用户名，模拟时在城市之间穿梭。当鼠标移至小圆圈时，在左侧的显示信息栏会展示该旅客当前的状况，信息包含当前的起止城市、交通方式及花费、起止时间，以及整个旅行的到达时间、花费和目的地。当想要查看某一个用户的信息时，只需将鼠标指向该用户的小圆圈即可在展示在左侧。
    ![](https://github.com/dachr8/Travel-Simulation-Query-System/blob/master/document/users-instructions/display.png?raw=true)
### 暂停按钮使用说明
- 在界面右上方，程序提供暂停模拟按钮，方便用户输入需求以及仔细观察当前的模拟状况。用户刷新界面或重启浏览器时，模拟程序默认暂停，避免用户错过模拟时间。
### 异常情况说明
- 当容器未启动或容器退出时，浏览器界面会变红，界面如下，以提示用户。通常情况下程序不会无故退出。
     ![](https://github.com/dachr8/Travel-Simulation-Query-System/blob/master/document/users-instructions/error.png?raw=true)
### 新增旅客操作说明
- 为实现多个旅客同时模拟的功能，用户只需重复提交旅行计划的操作步骤，唯一不同的是需要改变用户名，即可成功添加一名新的用户，在同一时间，所有有旅行计划的用户将会在地图中一同展示出来。
### 旅行策略选择说明
- 花费最少策略适用于想要最大限度缩小旅行开销的旅客，他所提供的方案耗时长、且可能转车次数较多，旅途会十分煎熬。
- 时间最短策略适用于想要最快完成旅途的旅客，方案花销很大但耗时短，航班之间休息时间较少。
- 限时最省策略适用于对时间需求大于花销需求的旅客，算法对于时间把控比较严苛，故对时间要求较为吃紧，且想要适当减少花销的用户推荐采用限时最省策略。
## 三、命令行界面功能说明
### 命令行功能说明
- 在命令行当中输入"check"以查看当前所有用户的状态，信息包含此趟行程的起止地点、时间、航班信息。
- 在命令行中输入某一用户的用户名，即可查看该用户的所有行程计划表，内容包含每一趟行程的全部信息。
- 在命令行中输入"#"以退出模拟旅行程序。
### 日志文件获取步骤
- 若用户未在进行模拟的过程中，则直接输入命令 `docker cp <container_id>:/opt/Travel-Simulation-Query-System/build/log.txt  <log_destination>`
    PS：第二个路径为存储路径，用户可以自主更改。
- 若用户处在模拟运行的过程中，首先输入Ctrl+P+Q，退出但不关闭container，之后再输入上述命令取出log文件。
- 在每次模拟过程退出之后，请及时将log日志文件取出，在下一次模拟开始后，log将会被自动覆盖。
