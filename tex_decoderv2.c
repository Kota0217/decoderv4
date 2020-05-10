//実験のデータ(.csv)をtexの表に落とし込むためのdecoder
//latexに表を入れられるように書式を作る
//横のラベルは個数だけ標準入力から入力、後はtxtファイルに手打ちで入力
//縦軸はy/nを問われるので、1/0を入力すると、左端のラベルが1からの連番になる
//ちょっとした変更

#include <stdio.h>
#include <stdlib.h>

int getl(char s[], int lim){
        int c, i = 0;
        for(c = getchar(); c != EOF && c != '\n'; c = getchar()) {
        s[i++] = c; if(i+1 >= lim) { break; } }
        s[i] = '\0'; return c != EOF; 
}

int main(int argc, char *argv[])
{

    //使う変数の宣言
    FILE *fr;                           //読み出すテキストデータのファイルを指すファイルポインタ
    FILE *fw;                           //書き出し先のファイルを指すファイルポインタ
    char wFileName[255] = "output.txt"; //出力ファイル名を保持する配列．標準ではoutput.txt
    int result;                         //いろいろな処理の結果を保持するための変数
    int  i = 0;                         //ループ用変数
    char buf;                           //csvファイルから読み込んだ文字を保持するための変数
    char input[100];                    //標準入力から読み込んだ文字を保持するための配列
    

    //引数の数が不足していた場合
    if (argc < 2)//do
    {
        fprintf(stderr, "Enter an input file name.\n");
        exit(EXIT_FAILURE);
    }
    else if (argc > 2)
    {                                      //出力先の指定がされている場合
        sprintf(wFileName, "%s", argv[2]); //出力先の指定を書き換える
    }

    //読み出し元のファイルを開く
    if ((fr = fopen(argv[1], "rb")) == NULL)
    { //ファイルが開けなかったら
        fprintf(stderr, "File not found:%s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    //書き出し先のファイルを開く
    if ((fw = fopen(wFileName, "w")) == NULL)
    { //ファイルが開けなかったら
        fprintf(stderr, "File not found:%s\n", wFileName);
        exit(EXIT_FAILURE);
    }


    /*for(int j = 0; j < i; j++){
    printf("label[%d],%c\n",i, label[i].name);
    }
    */
    //見栄えのため、表は中心におく
    fprintf(fw, "\\begin{center}\n");
    fprintf(fw, "\\begin{table}[]\n");
    fprintf(fw, "\\begin{tabular}{");

    //横labelの数だけ把握して、入力は手打ち
    printf("label_number >\n");
    //getsで文字列として読み込んでから、atoiで数値に変換
    //本当はgetsは関数として(色々な理由がある)安全ではないけど、とりあえず使う
    gets(input);
    i = atoi(input);
    //printf("%d\n", i);
    
    //lをi個配置する
    int j = 0;
    for(j=0; j < i; j++){
        fprintf(fw, "l");
    }
    
    fprintf(fw, "}\n");

    //縦軸のラベルに、番号振りが必要か入力する
    printf("縦軸ラベル 1/0 ? >\n");
    char ans[1];
    gets(ans);
    result = atoi(ans); //1なら縦ラベルを連番にする

    //横のラベルを表示
    for (j = 0; j < i-1; j++)
    {
        fprintf(fw, "label%d", j+1);
        fprintf(fw, " & ");
    }
        fprintf(fw, "label%d ", j+1);
        fprintf(fw, "\\");
        fprintf(fw, "\\ ");
        fprintf(fw, "\\hline");
        fprintf(fw, "\n");
    
    i = 1;  //連番用にiを1にリセット

    while (1)
    { //とりあえず無限ループ(終了条件は内部で定義)
        //ファイルから1文字読み込む
        buf = fgetc(fr);

        if (buf == EOF){
            break; //終端文字(データの終わり)が来たら変換終了
        }

        if (buf == 0x20 || buf == 0x2C)     //データの区切り(","または、スペース)が来たら区切り文字を入れる
        { //データの区切りまで読み続ける
            fprintf(fw, " & ");
        }
        
        else if(buf == 0x0a){        //改行がきたら\\を行末につけて、改行
            fprintf(fw, "\\");
            fprintf(fw, "\\");
            fprintf(fw, "\n");

            if (result == 1){       //連番が必要な場合
                fprintf(fw, "%d & ", i);
                i++;
            }
            
            
        }
        
        else
        {
            //putchar(buf);             //ターミナルに表示したい時はこっち
            fprintf(fw, "%c", buf);
            
        }

    }

    fprintf(fw, "\\end{tabular}\n");
    fprintf(fw, "\\end{table}\n");
    fprintf(fw, "\\end{center}\n");

    fclose(fw);

    //printf("decorded!!\n");
}
