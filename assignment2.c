#include <stdio.h>
#include <string.h>

typedef struct{
    int token;
    char name[10];  //name up to 11 char
    int val;        //number (ASCII value)
    int level;      //L level
    int adr;        //M address
}namerecord_t;

typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym } token_type;


void printSourceCode();
void printLexemeTable(namerecord_t table[], int index);
namerecord_t constFunc(FILE * ifp);
namerecord_t varFunc(FILE * ifp);


int main(void){

    int i, index=0, len;
    char c;
    FILE *ifp, *ofp;
    char reservedWords[14][9];
    char temp[14];
    namerecord_t table[1000];
    char tempString[10];


    ifp = fopen("reservedWords.txt", "r");
    if(ifp == NULL)
        printf("Error Opening File.\n");

    //fill in string array with reserved words
    for(i = 0; i < 14; i++){
        fscanf(ifp, "%s", temp);
        strcpy(reservedWords[i], temp);
    }

//    printSourceCode();

    //open input file
    ifp = fopen("input.txt", "r");
    if(ifp == NULL)
        printf("Error Opening File.\n");

    ofp = fopen("output.txt", "w");

        while(!feof(ifp)){
            //reset i to find it in reserved words list
           i = 0;
           //reset tempstring by putting endOfString character in the beginning
           tempString[0] = '\0';

            fscanf(ifp, "%s", tempString);

            while(strcmp(tempString, reservedWords[i])!=0){
                i++;
                if(i>14)
                    break;
            }

            switch(i){
                case 0://const
                    table[index].token = constsym;
                    strcpy(table[index].name,"const");
                    index++;
                    do{
                        table[index] = constFunc(ifp);
                        index++;
//                        printLexemeTable(table, index);
//                        system("pause");
                    }while(table[index-1].token != semicolonsym);

                break;
                case 1://var
                    table[index].token = varsym;
                    strcpy(table[index].name,"var");
                    index++;

                    do{
                        table[index] = varFunc(ifp);
                        printLexemeTable(table, index);
                        system("pause");
                        index++;
                    }while(table[index-1].token != semicolonsym);
                break;
                case 2://procedure
                break;
                case 3://call
                break;
                case 4://begin
                break;
                case 5://end
                break;
                case 6://if
                break;
                case 7://then
                break;
                case 8://else
                break;
                case 9://while
                break;
                case 10://do
                break;
                case 11://read
                break;
                case 12://write
                break;
                case 13://NULL
                break;
                default:
                break;
            }//end switch
        printLexemeTable(table, index);
        system("pause");
        }

    fclose(ifp);
    fclose(ofp);
}//end main

void append(char * string, char c){
    int len = strlen(string);
    string[len] = c;
    string[len+1] = '\0';
}

void printSourceCode(){

    //in final version make sure to print out to file instead of console

    FILE *ifp;
    char c;

    ifp = fopen("input.txt", "r");

    while(!feof(ifp)){
        fscanf(ifp, "%c", &c);
        printf("%c", c);
    }

    printf("\n\n");

}

void printLexemeTable(namerecord_t table[], int index){

    int i;
    printf("Lexeme Table\n");
    printf("Lexeme\tToken Type\n");

    for(i=0;i<index;i++){
        printf("%s\t%d\n", table[i].name, table[i].token);
    }

     printf("\n\n");
}

namerecord_t constFunc(FILE * ifp){

    namerecord_t temp;
    int flag = 0;
    char c;

    strcpy(temp.name, ""); //clears any data that may be in name
    temp.token = -1; //sets token to a undefined token type
    c = ' ';

    do {
        fscanf(ifp, "%c", &c);
    } while (c == ' ');//skips over all white space after const

    if(c == ','){
        append(temp.name, c);
        temp.token = commasym;
    }
    else if(c == '='){ //
        append(temp.name, c);
        temp.token = eqsym;
    }
    else if((int)c >= 48 && (int)c <=57){
        append(temp.name, c);
        temp.token = numbersym;
    }
    else if(c == ';'){
        append(temp.name, ';');
        temp.token = semicolonsym;
    }
    else{
        //find out how to make it work for A=1,Z=2;
        while(c != ' ' && c != ',' && c != '=' && c != ';'){
            append(temp.name, c);
            fscanf(ifp, "%c", &c);
            temp.token = identsym;
        }
    }

    return temp;
}

namerecord_t varFunc(FILE * ifp){

    FILE * tempFilePointer;
    namerecord_t temp;
    int flag = 0;
    char c, d;

    strcpy(temp.name, ""); //clears any data that may be in name
    temp.token = -1; //sets token to a undefined token type
    c = ' ';

    do {
        fscanf(ifp, "%c", &c);
    } while (c == ' ');//skips over all white space after const

    if(c == ','){
        append(temp.name, c);
        temp.token = commasym;
    }
//    else if((int)c >= 48 && (int)c <=57){
//        append(temp.name, c);
//        temp.token = numbersym;
//    }
    else if(c == ';'){
        append(temp.name, ';');
        temp.token = semicolonsym;
    }
    else{
        //find out how to make it work for A=1,Z=2;
        while(c != ' ' && c != ',' && c != ';'){
            append(temp.name, c);
            tempFilePointer = ifp;

            //i broke it here
            //tried to look ahead and see if the next character was a , = or ;
            fscanf(tempFilePointer, "%c", &d);
            if(d != ',' || d != '=' || d != ';')
                fscanf(ifp, "%c", &c);
            temp.token = identsym;
        }
    }

    return temp;
}
