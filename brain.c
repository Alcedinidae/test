#include <stdio.h>
#include <stdlib.h>

#define MEM 30000

/*命令の一覧*/
char op[8] = {'>', '<', '+', '-', '.', ',', '[', ']'};

/*メモリー*/
char mem[MEM] = {0};
char *s_bracket[1000];  //[の位置を格納する配列ポインタ
char *e_bracket;  //]の位置を格納するポインタ

/*関数の宣言*/
int analyze(char s[]);
int runcode(char *mem_ptr, char *inp_ptr, char *pos);

/*メイン関数*/
int main(int argc, char *argv[]){
    /*変数宣言*/
    int result;  //解析結果とラン結果
    char *position;  //ブラケットが命令文の何文字目かのポインタ
    char *mem_ptr;  //メモリのポインタ
    char *inp_ptr = argv[1];  //命令文のポインタ
    
    
    /*初期設定*/
    mem_ptr = mem;
    e_bracket = NULL;
    
    printf("analyze code...\n");
    
    /*入力された命令の解析*/
    result = analyze(argv[1]);
    if(result == 1){
        printf("syntax error\n");
        return 1;
    }
    printf("run code:\n");
    
    /*入力された命令の実行*/
    result = runcode(mem_ptr, inp_ptr, position);
    if(result == 1){
        printf("error\n");
        return 1;
    }

    return 0;
}

/*命令の解析*/
/*引数から受け取ったコマンドに不明な文字がないか検査する*/
/*もし不明な文字があれば1を返し、なければ0を返す*/
int analyze(char s[]){
    int i;
    char *p_str = s;
    while(*p_str != '\0'){
        /*命令の一覧を探索し最後の要素まで同じにならなかったら1を返す*/
        for(i=0;i<8;i++){
            if(op[i] == *p_str){
                break;
            }
        }
        if(i == 8){
            return 1;
        }
        p_str++;
    }
    return 0;
}

/*命令の実行*/
/*引数のコマンドを実行する*/
/*簡単なブラケットの検査はついているが無限ループになるかどうかの判定はまだ実装していない*/
/*先に命令を解析しているのでswitchのdefaultには通常入らないが念のため用意*/
/*実行失敗なら1を返し、成功なら0を返す*/
int runcode(char *mem_ptr, char *inp_ptr, char *pos){
    int s_index = 0;  //s_bracketのインデックス
    
    while(*inp_ptr != '\0'){
        switch(*inp_ptr){
            case '>':
                mem_ptr++;
                break;
            case '<':
                mem_ptr--;
                break;
            case '+':
                (*mem_ptr)++;
                break;
            case '-':
                (*mem_ptr)--;
                break;
            case '.':
                putchar(*mem_ptr);
                break;
            case ',':
                *mem_ptr = getchar();
                while(getchar() != '\n');
                break;
            case '[':
                /*ループカウンタが0になるとき*/
                if(*mem_ptr == 0){
                    /*]がなければエラー*/
                    if(e_bracket == NULL){
                        printf("bracket error!\n");
                        return 1;
                        }
                    else{
                    /*]があるならそこへジャンプ*/
                        inp_ptr = e_bracket;
                        e_bracket = NULL;
                        s_index--;
                    }
                }
                else{
                    s_bracket[s_index++] = inp_ptr; 
                }
                break;
            case ']':
                /*スタックに[がないならループが作れないのでエラー*/
                if(s_index == 0 || s_bracket[s_index - 1] == NULL){
                    printf("bracket error2!\n");
                    return 1;
                }
                /*[があるならスタックの内容を１つ消popする*/
                e_bracket = inp_ptr;
                inp_ptr = s_bracket[s_index - 1] - 1;
                break;
            default:
                printf("unexpected error!\n");
                return 1;    
        }
        inp_ptr++;
    }
    return 0;
}
