/*
    Robert Max Gomez
    Assignment 2
    COP 3402
    Summer 2013
*/

#include <stdio.h>
#include <string.h>

#define rwCount 29
#define maxWordLength 12
#define tablesize 1000

typedef struct
{
	int token; 		 //const = 1, var = 2, proc = 3
    char name[12];	 //name up to 11 chars
    int val; 		 //number (ASCII value)
    int level; 		 //L  level
    int adr; 		 //M  address
    } namerecord_t;

typedef enum {
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym } token_type;

void printSourceCode(FILE * ofp);
void printLexemeTable(namerecord_t table[], int index, FILE * ofp);
void append( char *str, char c);
int checkType(char temp[], char reservedWords[][maxWordLength]);
int checkReservedWord(char temp[], char reservedWords[][maxWordLength]);
void printLexemeList(namerecord_t table[], int index, FILE * ofp);

int main(void){
    char reservedWords[rwCount][maxWordLength];
    char c, temp[12] = "", d;
    namerecord_t table[tablesize];

    FILE *ifp, *rw, *ofp;
    int i = 0, index = 0;
    ofp = fopen("output.txt","w");

    //open reserved words
    rw = fopen("reservedWords.txt", "r");
    if(rw == NULL){
        printf("Error reading reserved words");
        return 0;
    }

    //reads in the list of reserved words from file
    while(!feof(rw)){
        fscanf(rw, "%s", &reservedWords[i][0]);
        i++;
    }

    //open input file
    ifp = fopen("input.txt", "r");
    if(ifp == NULL){
        printf("error reading input file");
        return 0;
    }

    //read in first character
    fscanf(ifp, "%c", &c);

    while(!feof(ifp)){

        //resets temp
        strcpy(temp, "");

        //skips white space, tabs and new lines
        if(c == ' ' || c == '\n' || c == '\t')
            do {
                fscanf(ifp, "%c", &c);
                //skips white space finds next char
            } while (c == ' ' || c == '\n' || c == '\t');

        //skips comments
        if (c == '/') {
            //checks if next character after the '/' is a '*'
            fscanf(ifp, "%c", &d);
            if(d == '*'){
                //skips everything until a * is found
                while (1) {
                    //c keeps track of previous character
                    //d gets the next character
                    c = d;
                    fscanf(ifp, "%c", &d);
                    //if c is a * and d is a / then the comment is over.
                    if(c == '*' && d == '/')
                        break;
                }//end while
            }//end if *
            else{
                //add c to temp
                append(temp, c);
                //copy temp
                strcpy(table[index].name, temp);
                table[index].token = slashsym;
                index++;
                //c = d because so we can set token for d
                c = d;
            continue;
            }
        }//end if /
        //if the nect character is a : it must be a := or else error
        else if(c == ':'){
            fscanf(ifp, "%c", &d);
            if(c == ':' && d == '='){
                append(temp, c);
                append(temp, d);
                strcpy(table[index].name, temp);
                table[index].token = becomessym;
                index++;
                fscanf(ifp,"%c", &c);
                continue;
            }
            else{
                printf("Error, Invalid Symbol");
                return 0;
            }
        }
        else if((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
            //reads in numbers and characters.
            while ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)) {
                append(temp, c);
                fscanf(ifp, "%c", &c);
            }
            i=0;
            //checks if its a number
            if(temp[i] >= 48 && temp[i] <= 57){
                //iterate through temp to see if first value is a number and the rest of them are numbers as well
                //if first value is a number and rest are letters error
                //checks by looking at first value, if it is a number go till the end of the string and iterate using i
                //by breaks out if there is a letter
                //if i != strlen(temp) by the end then there was a letter and throw an error
                while(temp[i] >= 48 && temp[i] <= 57 && i <= strlen(temp))
                    i++;
                if(i!=strlen(temp)){
                    printf("Error, Invalid Variable Name");
                    return 0;
                }
                else if (i > 5){
                    printf("Error, Integer longer than 5 digits");
                    return 0;
                }
                else{
                    strcpy(table[index].name, temp);
                    table[index].token = numbersym;
                    index++;
                    continue;
                }

            }
            //if its just a word
            else{
                //check if its a reserved word.
                if(checkReservedWord(temp, reservedWords) == -1){
                    if(strlen(temp) > 11){
                        printf("Variable Length Too Long");
                        return 0;
                    }
                    strcpy(table[index].name, temp);
                    table[index].token = identsym;
                    index++;
                    continue;
                }

            }

        }
        else{
            //builds other symbols
            append(temp, c);
            fscanf(ifp, "%c", &c);
        }

        strcpy(table[index].name, temp); //adds word to lexeme table
        table[index].token = checkType(temp, reservedWords);

        index++;

        }//end while not end of file

        //print out shit n shit
        printSourceCode(ofp);
        printLexemeTable(table, index, ofp);
        printLexemeList(table, index, ofp);


    return 0;
}

    void append( char *str, char c){
        int len = strlen(str);
        str[len] = c;
        str[len+1] = '\0';
    }

void printSourceCode(FILE * ofp){

    //in final version make sure to print out to file instead of console

    FILE *ifp;
    char c;

    ifp = fopen("input.txt", "r");

    do{
        fscanf(ifp, "%c", &c);
        fprintf(ofp, "%c", c);
    }while(c != '.');

    fprintf(ofp, "\n\n");

}

void printLexemeTable(namerecord_t table[], int index, FILE * ofp){

    int i;
    fprintf(ofp, "Lexeme Table\n");
    fprintf(ofp, "Lexeme\t\t\tToken Type\n");

    for(i=0;i<index;i++){
        if(strlen(table[i].name) > 8)
            fprintf(ofp, "%s\t\t%d\n", table[i].name, table[i].token);
        else
            fprintf(ofp, "%s\t\t\t%d\n", table[i].name, table[i].token);
    }

     fprintf(ofp, "\n\n");
}

void printLexemeList(namerecord_t table[], int index, FILE * ofp){

    int i;

    fprintf(ofp, "Lexeme List\n");

    for(i=0;i<index;i++){
        if(table[i].token == identsym)
            fprintf(ofp, "%d %s ", table[i].token, table[i].name);
        else
            fprintf(ofp, "%d ", table[i].token);
    }

}

int checkReservedWord(char temp[], char reservedWords[][maxWordLength]){
 int i = 0, flag = 0;


    while(i<rwCount){
        if(strcmp(temp,reservedWords[i]) == 0){
            flag = 1;
            break;
        }
        i++;
    }
    //flag == 1 when reserved word is found, then return where in reservedWords it was found
    if(flag == 1)
        return i;
    //if its not found return -1;
    else
        return -1;
}

int checkType(char temp[], char reservedWords[][maxWordLength]){

   int i = checkReservedWord(temp, reservedWords);

    switch(i){
        case 0://const
            return constsym;
            break;
        case 1://var
            return varsym;
            break;
        case 2://procedure
            return procsym;
            break;
        case 3://call
            return callsym;
            break;
        case 4://begin
            return beginsym;
            break;
        case 5://end
            return endsym;
            break;
        case 6://if
            return ifsym;
            break;
        case 7://then
            return thensym;
            break;
        case 8://else
            return elsesym;
            break;
        case 9://while
            return whilesym;
            break;
        case 10://do
            return dosym;
            break;
        case 11://read
            return readsym;
            break;
        case 12://write
            return writesym;
            break;
        case 13://+
            return plussym;
            break;
        case 14://-
            return minussym;
            break;
        case 15://*
            return multsym;
            break;
        case 16:// /
            return slashsym;
            break;
        case 17://(
            return lparentsym;
            break;
        case 18://)
            return rparentsym;
            break;
        case 19://=
            return eqsym;
            break;
        case 20://,
            return commasym;
            break;
        case 21://.
            return periodsym;
            break;
        case 22://:=
            return becomessym;
            break;
        case 23://<
            return lessym;
            break;
        case 24://<=
            return leqsym;
            break;
        case 25://>
            return gtrsym;
            break;
        case 26://>=
            return geqsym;
            break;
        case 27://;
            return semicolonsym;
            break;
        case 28://NULL
            return nulsym;
            break;
    }

}




