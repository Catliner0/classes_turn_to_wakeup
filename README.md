# classes_turn_to_wakeup  
[![wakeup课程表](https://img.shields.io/badge/wakeup课程表-v1.0-pink)](https://www.wakeup.fun/)  
一个基于C语言构筑的程序，将从CCUT（长春工业大学）教务系统网页端下载的课表转化为可供wakeup课程表app导入的CSV文件  
`本项目已停止维护更新`  
## 下载指南    
在本仓库 `main/release` 目录下下载 `main.exe`  
## 使用指南  
>1.从长春工业大学教务系统下载课表`download.xls`文件  
>2.使用MSoffice工具将.xls 转换成UTF-8 CSV文件，保存为`download.csv`  
>3.将`download.csv`与`main.exe`文件放在同一目录，运行`main.exe`  
>4.将生成的`classes.csv`在QQ里用`wakeup课程表打开`，导入wakeup课程表  

## 版本说明  
 - `main2.exe`会将相同课程的周数合并    
 - 这只是小完善，`main.exe`打印的课表也可以导入   
