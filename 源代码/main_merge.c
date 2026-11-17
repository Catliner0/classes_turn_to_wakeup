#include<stdio.h> 
#include<string.h>
#include<windows.h>
#include<stdlib.h>
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

/* ==================== 【新增代码：开始】 ====================
 * 用途：把已经打印好的 classes.csv 里“只有周数不同”的行合并成一行，
 *       周数之间用“、”隔开。例如：
 *           微积分4[基础理论],2,1,2,未知,综合楼-第一阶梯,2
 *           微积分4[基础理论],2,1,2,未知,综合楼-第一阶梯,3
 *       合并为：
 *           微积分4[基础理论],2,1,2,未知,综合楼-第一阶梯,2、3
 *
 * 做法：
 *   1. 逐行读入 classes.csv，以“最后一个逗号”为界：
 *        逗号左边 -> 课程名称,星期,开始节数,结束节数,老师,地点（合并依据）
 *        逗号右边 -> 周数字段
 *   2. 左边内容完全相同的行归为一组，周数收进同一个集合（去重、升序）。
 *   3. 每组输出一行，周数用“、”连接，组的先后顺序 = 每组第一次出现的顺序。
 *   4. 先写临时文件 classes.csv.tmp，成功后再替换 classes.csv，
 *      避免中途出错把原表写坏。
 *
 * 原有的 download.csv 解析、classes.csv 打印逻辑一行都没改，
 * 只在 main() 里多加了一次 merge_csv_weeks("classes.csv") 调用。
 * ============================================================ */

#define MERGE_LINE_MAX  4096    /* 单行最大长度 */
#define MERGE_WEEK_MAX  256     /* 单组最多保存多少个周数 */

typedef struct{
    char *key;                   /* 课程名称,星期,开始节数,结束节数,老师,地点 */
    int   week[MERGE_WEEK_MAX];  /* 周数集合：升序、不重复 */
    int   weeknum;
}merge_group;

/* 去掉首尾的空格、制表符和回车换行，返回处理后的长度 */
static int merge_trim(char *s)
{
    char *p = s;
    size_t len;
    while(*p == ' ' || *p == '\t'){
        p++;
    }
    if(p != s){
        memmove(s, p, strlen(p) + 1);
    }
    len = strlen(s);
    while(len > 0 && (s[len-1] == '\r' || s[len-1] == '\n' ||
                      s[len-1] == ' '  || s[len-1] == '\t')){
        s[--len] = '\0';
    }
    return (int)len;
}

/* 往组里加一个周数：升序插入，已存在就忽略 */
static void merge_add_week(merge_group *g, int w)
{
    int i;
    if(w < 0 || w > 9999){
        return;
    }
    for(i = 0; i < g->weeknum; i++){
        if(g->week[i] == w){          /* 去重 */
            return;
        }
        if(g->week[i] > w){           /* 找到插入位置 */
            break;
        }
    }
    if(g->weeknum >= MERGE_WEEK_MAX){
        return;
    }
    memmove(&g->week[i+1], &g->week[i], (size_t)(g->weeknum - i) * sizeof(int));
    g->week[i] = w;
    g->weeknum++;
}

/* 判断 p 处是不是分隔符/可忽略字符。
 * 注意：中文标点在 UTF-8 里是多字节，必须按字符串比较，
 *       不能写成 *p == '、'（单个 char 装不下多字节字符）。
 * 命中时通过 len 返回该字符占用的字节数。 */
static int merge_is_sep(const char *p, int *len)
{
    static const char *seps[] = {
        "、", "，", ",", ";", "；", "～", " ", "\t", "\"", "周", NULL
    };
    int i;
    for(i = 0; seps[i] != NULL; i++){
        size_t n = strlen(seps[i]);
        if(strncmp(p, seps[i], n) == 0){
            *len = (int)n;
            return 1;
        }
    }
    return 0;
}

/* 解析周数字段，例如 "3" / "1、2、3" / "1-16" / "1、3-5、8"
 * 返回识别到的周数个数（0 表示这一行没有周数） */
static int merge_parse_weeks(const char *field, merge_group *g)
{
    const char *p = field;
    int count = 0;

    while(*p){
        long a, b;
        char *end = NULL;
        int sep_len = 1;

        /* 跳过各种分隔符，包括已经合并过的“、” */
        if(merge_is_sep(p, &sep_len)){
            p += sep_len;
            continue;
        }

        a = strtol(p, &end, 10);
        if(end == p){                 /* 不是数字，跳过这个字符 */
            p++;
            continue;
        }
        p = end;
        b = a;

        /* 支持 “a-b” / “a～b” 区间写法 */
        {
            const char *q = p;
            int dash_len = 0;
            while(*q == ' ' || *q == '\t'){
                q++;
            }
            if(strncmp(q, "-", 1) == 0 || strncmp(q, "~", 1) == 0){
                dash_len = 1;
            }else if(strncmp(q, "～", 3) == 0){
                dash_len = 3;
            }
            if(dash_len > 0){
                char *end2 = NULL;
                long v = strtol(q + dash_len, &end2, 10);
                if(end2 != q + dash_len){
                    b = v;
                    p = end2;
                }
            }
        }

        if(a < 0){
            a = 0;
        }
        if(b < a){
            b = a;
        }
        if(b - a > 60){               /* 防止异常数据把数组撑爆 */
            b = a + 60;
        }
        for(; a <= b; a++){
            merge_add_week(g, (int)a);
        }
        count++;
    }
    return count;
}

/* 按 key 查找已有组，找不到返回 NULL */
static merge_group *merge_find(merge_group *groups, int num, const char *key)
{
    int i;
    for(i = 0; i < num; i++){
        if(strcmp(groups[i].key, key) == 0){
            return &groups[i];
        }
    }
    return NULL;
}

/* 新建一组（数组不够就扩容），失败返回 NULL */
static merge_group *merge_push(merge_group **arr, int *num, int *cap, const char *key)
{
    merge_group *g;
    if(*num >= *cap){
        int newcap = (*cap == 0) ? 256 : (*cap) * 2;
        merge_group *p = (merge_group*)realloc(*arr, (size_t)newcap * sizeof(merge_group));
        if(p == NULL){
            return NULL;
        }
        *arr = p;
        *cap = newcap;
    }
    g = &(*arr)[*num];
    g->key = (char*)malloc(strlen(key) + 1);
    if(g->key == NULL){
        return NULL;
    }
    strcpy(g->key, key);
    g->weeknum = 0;
    (*num)++;
    return g;
}

/* 释放所有组的 key */
static void merge_free(merge_group *groups, int num)
{
    int i;
    for(i = 0; i < num; i++){
        free(groups[i].key);
    }
    free(groups);
}

/* 主函数：合并 classes.csv 的周数（就地覆盖该文件） */
int merge_csv_weeks(const char *path)
{
    char tmp_path[600];
    char line[MERGE_LINE_MAX];
    char header[MERGE_LINE_MAX] = "";
    merge_group *groups = NULL;
    int group_num = 0, group_cap = 0;
    int has_bom = 0, have_header = 0;
    int line_num = 0, data_num = 0, bad_line = 0;
    FILE *in = NULL, *out = NULL;
    int i, k;

    in = fopen(path, "rb");
    if(in == NULL){
        printf("周数合并：打不开 %s，跳过合并。\n", path);
        return -1;
    }

    while(fgets(line, (int)sizeof(line), in) != NULL){
        char *week_field = NULL;
        char *comma = NULL;
        merge_group *g = NULL;

        line_num++;

        /* 第一行的 UTF-8 BOM 先摘掉，写回时再补上 */
        if(line_num == 1 &&
           (unsigned char)line[0] == 0xEF &&
           (unsigned char)line[1] == 0xBB &&
           (unsigned char)line[2] == 0xBF){
            memmove(line, line + 3, strlen(line + 3) + 1);
            has_bom = 1;
        }

        if(merge_trim(line) == 0){    /* 空行直接丢掉 */
            continue;
        }

        if(!have_header){             /* 第一行是表头，原样保留 */
            strcpy(header, line);
            have_header = 1;
            continue;
        }

        comma = strrchr(line, ',');   /* 从最后一个逗号切开 */
        if(comma == NULL){
            bad_line++;
            continue;
        }
        *comma = '\0';
        week_field = comma + 1;
        merge_trim(line);
        merge_trim(week_field);

        g = merge_find(groups, group_num, line);
        if(g == NULL){
            g = merge_push(&groups, &group_num, &group_cap, line);
            if(g == NULL){
                fclose(in);
                merge_free(groups, group_num);
                printf("周数合并：内存不足，已放弃合并。\n");
                return -1;
            }
        }
        merge_parse_weeks(week_field, g);
        data_num++;
    }
    fclose(in);

    /* 先写临时文件，再整体替换，防止写一半把原表毁掉 */
    snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path);
    out = fopen(tmp_path, "wb");
    if(out == NULL){
        merge_free(groups, group_num);
        printf("周数合并：无法创建 %s，已放弃合并。\n", tmp_path);
        return -1;
    }

    if(has_bom){
        fputs("\xEF\xBB\xBF", out);   /* 保持原来的 BOM，Excel / wakeup 才认得 UTF-8 */
    }
    if(have_header){
        fputs(header, out);
        fputc('\n', out);
    }
    for(i = 0; i < group_num; i++){
        fputs(groups[i].key, out);
        fputc(',', out);
        for(k = 0; k < groups[i].weeknum; k++){
            if(k > 0){
                fputs("、", out);
            }
            fprintf(out, "%d", groups[i].week[k]);
        }
        fputc('\n', out);
    }
    fclose(out);

    remove(path);                     /* Windows 下 rename 不能覆盖已存在的文件 */
    if(rename(tmp_path, path) != 0){
        merge_free(groups, group_num);
        printf("周数合并：写回 %s 失败，合并结果留在 %s。\n", path, tmp_path);
        return -1;
    }

    printf("周数合并完成：%d 行 -> %d 行", data_num, group_num);
    if(bad_line > 0){
        printf("（其中 %d 行格式异常已跳过）", bad_line);
    }
    printf("。\n");

    merge_free(groups, group_num);
    return 0;
}
/* ==================== 【新增代码：结束】 ==================== */

int main(){//主程序
    SetConsoleOutputCP(65001);//格式化控制台输出格式
    csvget();
    merge_csv_weeks("classes.csv");//【新增】把只有周数不同的行合并起来
    printf("打印完成！");
    return 0;
} 
