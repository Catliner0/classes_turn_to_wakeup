#include<stdio.h> 
#include<string.h>
#include<windows.h>
typedef struct{
    char name[80];
    int day;//星期
    int ti_s;//开始节数
    int ti_e;//结束节数
    char teacher[80];//老师
    char adress[80];//地点
    int week[20];//周
    int weeknum;
}class;
int rweek(int week[20]){//给week[20]数组的每个值赋值为-1
    int i;
    for(i=0;i<20;i++){
        week[i]=-1;
    }
    return 0;
}

int w_tows_(int a[],int size){//将整数数组week[size]转换为weeks[200]并打印在weeks.txt
    char weeks[200]="",dun[5]="、",str[20];
    int i;
    for(i=0;i<size;i++){
        snprintf(str,sizeof(str),"%d",a[i]);
        strcat(weeks,str);
        strcat(weeks,dun);
    }
    int len;
    len=strlen(weeks);
    if(len>0){
        weeks[len-3]='\0';
    }
    FILE*file=fopen("weeks.txt","w");
    if (file==NULL){//检测创建文件是否失败
        return -1;
    }
    fprintf(file,"%s\n",weeks);
    fclose(file);
    return 0;
}

int printcsv(class class[],int size){//将class class[]打印为classes.csv
    int i;
    char start[200]="课程名称,星期,开始节数,结束节数,老师,地点,周数";
    FILE *files=fopen("classes.csv","w");
    fprintf(files,"\xEF\xBB\xBF");
    fprintf(files,"%s\n",start);
    fclose(files);  
    char str[1000]="",ints[20],str1[200],dou[5]=",",none[2]="";
    for(i=0;i<size;i++){
        strcpy(str,none);
        strcat(str,class[i].name);
        strcat(str,dou);
        snprintf(ints,sizeof(str),"%d",class[i].day);
        strcat(str,ints);
        strcat(str,dou);
        snprintf(ints,sizeof(str),"%d",class[i].ti_s);
        strcat(str,ints);
        strcat(str,dou);
        snprintf(ints,sizeof(str),"%d",class[i].ti_e);
        strcat(str,ints);
        strcat(str,dou);
        strcat(str,class[i].teacher);
        strcat(str,dou);
        strcat(str,class[i].adress);
        strcat(str,dou);
        w_tows_(class[i].week,class[i].weeknum);
        FILE *fp=fopen("weeks.txt","r");
        fgets(str1,sizeof(str1),fp);
        strcat(str,str1);
        fclose(fp);
        FILE *f=fopen("classes.csv","a");
        fprintf(f,"%s",str);
        fclose(f);
    }
    return 0;
}

int csvget(){//识别download.csv
    //声明变量
    static char line[200],line_[200];//字符串缓存
    int line_num=0;//读取的csv文件行数
    int ti_s=0,ti_e=0,day=1,wk=-2,wk_,week[20];//class参数
    int mod=0,mod2=0;//模式参数
    int i,j=0,t;//循环参数
    static class classes[1000];//class结构体
    FILE *file=fopen("download.csv","r");//打开文件
    while(fgets(line,sizeof(line),file)){//逐行读取csv文件，临时保存在line
        line_num++;
        //去除换行符
        {size_t len=strlen(line);
        if(len>0 && line[len-1]=='\n'){
            line[len-1]='\0';
        }}
        //寻找“周次”
        if(strcmp(line,"\"周次")==0){
            wk=wk+3;
            wk_=wk;//wk_：临时周数
            mod=1;//开始识别程序
            ti_s=-1;
        }
        if(mod==1){//识别程序
            //寻找“第”
            {strcpy(line_,line);
            line_[3]='\0' ;}
            if(strcmp(line_,"第")==0){//开启课程识别模式
                strcpy(line_,line);
                //获取第n大节 后的，数量 为i
                for(i=0;i<20;i++){
                    if(line_[12+i]=='\"'){
                        break;
                    }
                }
                {day=i;//相关赋值
                mod2=1;
                ti_s=ti_s+2;
                ti_e=ti_s+1;
                wk_=wk;}
                if(i>19){//异常 关闭课程识别模式
                    mod=0;
                    mod2=0;
                }
                continue;
            }
            if(mod2>0){//课程识别程序
                mod2++;//mod2：课程信息5行一组；mod2=行数+1
                if(mod2==2){//第一行：课程名称
                    //课程名称赋值
                    strcpy(classes[j].name,line);
                    //其余int量赋值
                    classes[j].day=day;
                    classes[j].weeknum=1;
                    classes[j].ti_s=ti_s;
                    classes[j].ti_e=ti_e;
                    //week[20]={wk_}赋值
                    rweek(week);
                    week[0]=wk_;
                    memcpy(classes[j].week,week,20);
                }
                if(mod2==3){//第二行：教师
                    strcpy(classes[j].teacher,line);
                }
                if(mod2==5){//第三行：地点
                    strcpy(classes[j].adress,line);
                }
                if(mod2==6){//第五行：间隔（需计算星期day）
                    mod2=1;//mod2赋值还原
                    j++;//课程节数
                    //文字输出
                    printf("正在打印第%d节课程：%s\n",j,classes[j-1].name);
                    //获取逗号数
                    strcpy(line_,line);
                    for(t=0;t<100;t++){//寻找"
                        if(line_[t]=='\"'){
                            break;
                        }}
                    for(i=0;i<20;i++){
                        if(line_[t+i+1]=='\"'){
                            break;
                        }}
                    //day赋值
                    day=day+i;
                    if(day>7){//临时周数还原
                        t=day/7;
                        day=day-7*t;
                        wk_=wk_+t;
                    }
                
                }
            }
        }
        if(line_num==-1){break;}//测试代码：读取到n行结束
    }
    fclose(file);//关闭文件
    printcsv(classes,j);
    return 0;
}

int main(){//主程序
    SetConsoleOutputCP(65001);//格式化控制台输出格式
    csvget();
    printf("打印完成！");
    return 0;
} 




int classid_set(){//设置课程节数编号存储在classid.txt
    //classid.txt存储标准格式：单行为本节开始时间，双行为本节结束时间
    int num,i;
    double start,end;
    //获取循环终值
    printf("请输入每日课节数：");
    scanf("%d",&num);
    //清空原文件
    FILE*file=fopen("classid.txt","w");
    if (file==NULL){//检测创建文件是否失败
        return -1;
    }
    fclose(file);
    //获取内容并写入
    for(i=0;i<num;i++){
        printf("请输入第%d节课的开始时间：",i+1);
        scanf("%lf",&start);
        printf("请输入第%d节课的结束时间：",i+1);
        scanf("%lf",&end);
        FILE*file=fopen("classid.txt","a");
        fprintf(file,"%f\n%f\n",start,end);
        fclose(file);
    }
    return 0;
}