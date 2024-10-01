# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <cstdio> // sprintf
# include <map>
# include <stack>
# include <sstream> //
# include <vector>
# include <iostream> // strtof
# include <cmath> // floor
# include <string>
using namespace std;
enum TokenType
{
  IDENTIFIER = 17222,
  CONSTANT = 17223, // e.g., 35, 35.67, 'a', "Hi, there", true, false
  //       .35, 35., 0023
  SIGN = 23456,
  QUIT = 33333,
  OTHERS = 11111,
  UNRECOGNIZE = 22212,

  INT = 10000, // int
  FLOAT = 10001, // float
  CHAR = 10002, // char
  BOOL = 10003, // bool
  STRING = 10004, // string
  VOID = 10005, // void
  IF = 10006, // if
  ELSE = 10007, // else
  WHILE = 10008, // while
  DO = 10009, // do
  RETURN = 10010, // return
  GE = 10011, // >=
  LE = 10012, // <=
  EQ = 10013, // ==
  NEQ = 10014, // !=
  AND = 10015, // &&
  OR = 10016, // ||
  PE = 10017, // +=
  ME = 10018, // -=
  TE = 10019, // *=
  DE = 10020, // /=
  RE = 10021, // %=
  PP = 10022, // ++
  MM = 10023,  // --
  RS = 10024, // >>
  LS = 10025 // <<

} ;
typedef char * CharPtr ;

struct Column {
  int column ;
  Column * next ;
} ; // Column

typedef Column * ColumnPtr ;

struct Line {
  int line ;
  ColumnPtr firstAppearAt ;
  ColumnPtr lastAppearAt ;
  Line * next ;
} ; // Line

typedef Line * LinePtr ;

struct Token {
  CharPtr tokenStr ;
  TokenType type ;
  LinePtr firstAppearOn ;
  LinePtr lastAppearOn ;
  Token * next ;
} ; // Token

typedef Token * TokenPtr ;

struct Fun {
  string token;
  TokenType type;
};

struct ArrayID { // int a[50]
  string id; // a
  string arr; // [50]
};
struct ID {
  string tokenName; // function name
  string arr; // for int a[23]
  TokenType type;
  double value;
  bool again; // 重複定義
  vector<Fun> content; //  content
};

typedef char Str400[400];
typedef char Str10[10];
typedef char Str20[20];
static int uTestNum = -1 ;

int gLine = 1 ;
int gColumn = 1 ;
int gTest = 0;
char gNextChar = '\0' ;
int gNextCharLine = -1 ;
int gNextCharColumn = -1 ;
bool gNotFinish = false;
bool gerror = false;
bool gFirstTokenDone = false;
bool gDone = false;
bool gPrint = false; // check if print >
bool gKeep = true;
CharPtr gKeepToken = NULL;
TokenType gType;
TokenPtr gFront = NULL ;

vector<ID> gIDTable; // global define
vector<ID> gFunTable; // global function define
vector< vector<ArrayID> > gLocIDTb; // local id table

void GetNextChar( char & ch, int & line, int & column ) ;
CharPtr GetToken( TokenPtr head, TokenType & tokenType, int & firstCharLine, int & firstCharColumn ) ;
void OrderInsertToken( TokenPtr & head, CharPtr tokenStr,
                       TokenType tokenType, int tokenLine, int tokenColumn ) ;

void GetTokenInfo( TokenPtr head ) ;
void DeleteAllTokens( TokenPtr &head ) ;
bool Grammar( TokenPtr listHead ) ;
bool GetNextToken( TokenPtr &head ) ;
void UnexpToken( TokenPtr head ) ;
bool PeekNextToken( TokenPtr head, TokenPtr &peekToken ) ;
void UndefinedToken( TokenPtr head ) ;
void Reset( TokenPtr &head ) ;

bool CheckConstant( TokenPtr &head ) ;
bool User_input( TokenPtr &head ) ;
bool Definition( TokenPtr &head ) ;
bool Type_specifier( TokenPtr &head ) ;
bool Function_definition_or_declarators( TokenPtr &head ) ;
bool Rest_of_declarators( TokenPtr &head, vector<ArrayID> &IDvec ) ;
bool Statement( TokenPtr &head ) ;
bool Function_definition_without_ID( TokenPtr &head ) ;
bool Formal_parameter_list( TokenPtr &head ) ;
bool Compound_statement( TokenPtr &head ) ;
bool Declaration( TokenPtr &head, vector<ArrayID> &local ) ;
bool Statement( TokenPtr &head ) ;
bool CheckExpression( TokenPtr &head ) ;
bool Expression( TokenPtr &head ) ;
bool Basic_expression( TokenPtr &head ) ;
bool Rest_of_Identifier_started_basic_exp( TokenPtr &head ) ;
bool Rest_of_PPMM_Identifier_started_basic_exp( TokenPtr &head ) ;
bool Romce_and_romloe( TokenPtr &head ) ;
bool Actual_parameter_list( TokenPtr &head ) ;
bool Assignment_operator( TokenPtr &head ) ;
bool Rest_of_maybe_logical_OR_exp( TokenPtr &head ) ;
bool Maybe_logical_AND_exp( TokenPtr &head ) ;
bool Rest_of_maybe_logical_AND_exp( TokenPtr &head ) ;
bool Maybe_bit_OR_exp( TokenPtr &head ) ;
bool Rest_of_maybe_bit_OR_exp( TokenPtr &head ) ;
bool Maybe_bit_ex_OR_exp( TokenPtr &head ) ;
bool Rest_of_maybe_bit_ex_OR_exp( TokenPtr &head ) ;
bool Maybe_bit_AND_exp( TokenPtr &head ) ;
bool Rest_of_maybe_bit_AND_exp( TokenPtr &head ) ;
bool Maybe_equality_exp( TokenPtr &head ) ;
bool Rest_of_maybe_equality_exp( TokenPtr &head ) ;
bool Maybe_relational_exp( TokenPtr &head ) ;
bool Rest_of_maybe_relational_exp( TokenPtr &head ) ;
bool Maybe_shift_exp( TokenPtr &head ) ;
bool Rest_of_maybe_shift_exp( TokenPtr &head ) ;
bool Maybe_additive_exp( TokenPtr &head ) ;
bool Rest_of_maybe_additive_exp( TokenPtr &head ) ;
bool Maybe_mult_exp( TokenPtr &head ) ;
bool Rest_of_maybe_mult_exp( TokenPtr &head ) ;
bool Unary_exp( TokenPtr &head ) ;
bool Signed_unary_exp( TokenPtr &head ) ;
bool Unsigned_unary_exp( TokenPtr &head ) ;
void BuildGloFun( TokenPtr head, CharPtr FunName ) ;
void BuildGloID( TokenPtr head, vector<ArrayID> gloIDvec ) ;
void BuildSpeInput() ;

void GetNextChar( char & ch, int & line, int & column ) {
  scanf( "%c", &ch );
  line = gLine;
  column = gColumn;
  gColumn ++;
  if ( ch == '\n' ) {
    gLine ++;
    gColumn = 1;
  } // if
} // GetNextChar()

bool IsDigit( char ch ) {
  if ( ch >= 48 && ch <= 57 )  return true;
  else return false;
} // IsDigit()

bool IsLetter( char ch ) {
  if ( ( ch >= 65 && ch <= 90 ) || ( ch >= 97 && ch <= 122 ) )  return true;
  else   return false;
} // IsLetter()

bool IsWhiteSpace( char ch ) {
  if ( ch == ' ' || ch == '\n' || ch == '\t' )  return true;
  else return false;
} // IsWhiteSpace()

bool IsOthers( char ch ) {
  if ( ch == ':' || ch == '<' || ch == '>' || ch == '=' || ch == '(' || ch == ')' || ch == '*'
       || ch == '/' || ch == ';' || ch == '[' || ch == ']' || ch == '{' || ch == '}'
       ||  ch == '%' || ch == '^' || ch == '?' || ch == ','  || ch == '!'
       || ch == '|' || ch == '&' || ch == '+' || ch == '-' )  return true;
  else return false;
} // IsOthers()

void SkipWhiteSpaces() {
  while ( gNextChar  == ' ' || gNextChar == '\n' || gNextChar == '\t' ) {
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
  } // while
} // SkipWhiteSpaces()

void SkipLineComment() {
  while ( gNextChar != '\n' ) {
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
  } // while

  gColumn = 1;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
} // SkipLineComment()

CharPtr GetID() {
  Str400 name;
  int i = 1;
  name[0] = gNextChar;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  while ( IsWhiteSpace( gNextChar ) == false && i < 399 &&
          ( IsDigit( gNextChar ) == true ||  IsLetter( gNextChar ) == true || gNextChar == '_' ) ) {
    name[i] = gNextChar;
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    i ++;
  } // while

  name[i] = '\0';
  CharPtr tokenName = new char[i + 1];
  strcpy( tokenName, name ) ;
  return tokenName ;

} // GetID()

CharPtr GetNum() {
  Str400 name;
  int i = 1;
  bool isDot = false;
  name[0] = gNextChar;
  if ( gNextChar == '.' ) isDot = true;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  if ( name[0] == '.' && gNextChar == '.' ) return NULL;
  while ( !IsWhiteSpace( gNextChar ) && i < 399 &&
          ( IsDigit( gNextChar ) || ( gNextChar == '.' && !isDot ) ) ) {
    // '2...3' Unrecognized token with first char : '.'
    if ( gNextChar == '.' ) isDot = true;
    name[i] = gNextChar;
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    i ++;
  } // while

  name[i] = '\0';
  CharPtr tokenName = new char[i + 2];
  strcpy( tokenName, name ) ;
  return tokenName ;
} // GetNum()

CharPtr GetOthers( TokenType & tokenType ) {
  Str10 name;
  name[0] = gNextChar;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  if ( name[0] == '<' && ( gNextChar == '=' || gNextChar == '<' ) ) {
    if ( gNextChar == '=' ) tokenType = LE; // <=
    else if ( gNextChar == '<' ) tokenType = LS; // <<
  } // if
  else if ( name[0] == '>' && ( gNextChar == '=' || gNextChar == '>' ) ) {
    if ( gNextChar == '=' ) tokenType = GE; // >=
    else if ( gNextChar == '>' ) tokenType = RS; // >>
  } // else if
  else if ( ( name[0] == '+' || name[0] == '-' || name[0] == '*'
              || name[0] == '/' || name[0] == '%' || name[0] == '!' || name[0] == '=' )
            &&  gNextChar == '=' ) {
    if ( name[0] == '+' ) tokenType = PE; // +=
    else if ( name[0] == '-' ) tokenType = ME; // -=
    else if ( name[0] == '*' ) tokenType = TE; // *=
    else if ( name[0] == '/' ) tokenType = DE; // /=
    else if ( name[0] == '%' ) tokenType = RE; // %=
    else if ( name[0] == '!' ) tokenType = NEQ; // !=
    else if ( name[0] == '=' ) tokenType = EQ; // ==
  } // else if
  else if ( name[0] == '+' && gNextChar == '+' ) tokenType = PP;
  else if ( name[0] == '-' && gNextChar == '-' ) tokenType = MM;
  else if ( name[0] == '&' && gNextChar == '&' ) tokenType = AND;
  else if ( name[0] == '|' && gNextChar == '|' ) tokenType = OR;
  else {
    name[1] = '\0';
    CharPtr tokenName = new char[2];
    strcpy( tokenName, name ) ;
    tokenType = OTHERS;
    return tokenName ;
  } // else

  name[1] = gNextChar;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ); // name size = 2 get next char
  name[2] = '\0';
  CharPtr tokenName = new char[3];
  strcpy( tokenName, name ) ;
  return tokenName ;
} // GetOthers()

CharPtr GetString() {
  Str400 name;
  int i = 0;
  name[i] = gNextChar;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  i++;
  name[i] = gNextChar;
  while ( gNextChar != '\"' ) {
    scanf( "%c", &gNextChar ); // 處理換行字串
    if ( gNextChar == '\n' ) {
      gLine++;
      gColumn = 0;
    } // if

    i ++;
    name[i] = gNextChar;
    gColumn++;
    // GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );

  } // while

  i++;
  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ); // 取 " 下一個char
  name[i] = '\0';
  CharPtr tokenName = new char[i + 2];
  strcpy( tokenName, name ) ;
  return tokenName ;
} // GetString()

CharPtr GetCharConstant( TokenType & tokenType ) {
  Str10 c;
  c[0] = '\'';
  scanf( "%c", &gNextChar );
  if ( gNextChar == '\'' || gNextChar == '\n' ) tokenType = UNRECOGNIZE;
  else {
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
    c[1] = gNextChar;
    if ( gNextChar != '\'' ) {
      c[1] = '\0';
      tokenType = UNRECOGNIZE;
    } // if
    else {
      c[2] = gNextChar;
      c[3] = '\0';
      tokenType = CONSTANT;
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ); // 取 " 下一個char
    } // else
  } // else

  CharPtr tokenName = new char[5];
  strcpy( tokenName, c );
  return tokenName;
} // GetCharConstant()

CharPtr GetToken( TokenPtr head, TokenType & tokenType, int & firstCharLine, int & firstCharColumn ) {
  Str400 name;
  Str10 c;
  char peekChar = '\0';
  bool newLine = false;
  CharPtr tokenName;
  // printf("%c\n",gNextChar);
  while ( IsWhiteSpace( gNextChar ) ||  gNextChar == '/'  ) {
    if ( gColumn == 2 ) newLine = true;
    if ( IsWhiteSpace( gNextChar ) )  SkipWhiteSpaces() ;
    else if ( gNextChar == '/' ) {
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
      if ( gNextChar == '/' )   {
        if ( gLine == 1 && head == NULL && !newLine ) {
          gLine --; // 上一個敘述的註解
        } // if

        SkipLineComment() ;
      } // if
      else {
        c[0] = '/';
        c[1] = '\0';
        tokenName = new char[2];
        strcpy( tokenName, c );
        tokenType = OTHERS;
        return tokenName;
      } // else
    } // else if gNextChar is '/'
  } // while

  firstCharLine = gNextCharLine;
  firstCharColumn = gNextCharColumn;
  if ( IsLetter( gNextChar ) ) { // ignore first char with '_'
    tokenName = GetID();
    if ( strcmp( tokenName, "int" ) == 0 ) tokenType = INT;
    else if ( strcmp( tokenName, "float" ) == 0 ) tokenType = FLOAT;
    else if ( strcmp( tokenName, "char" ) == 0 ) tokenType = CHAR;
    else if ( strcmp( tokenName, "bool" ) == 0 ) tokenType = BOOL;
    else if ( strcmp( tokenName, "string" ) == 0 ) tokenType = STRING;
    else if ( strcmp( tokenName, "void" ) == 0 ) tokenType = VOID;
    else if ( strcmp( tokenName, "if" ) == 0 ) tokenType = IF;
    else if ( strcmp( tokenName, "else" ) == 0 ) tokenType = ELSE;
    else if ( strcmp( tokenName, "while" ) == 0 ) tokenType = WHILE;
    else if ( strcmp( tokenName, "do" ) == 0 ) tokenType = DO;
    else if ( strcmp( tokenName, "return" ) == 0 ) tokenType = RETURN;
    else if ( strcmp( tokenName, "true" ) == 0 || strcmp( tokenName, "false" ) == 0 ) {
      tokenType = CONSTANT;
    } // else if
    else  tokenType = IDENTIFIER;
  } // if
  else if ( gNextChar == '\"' ) { // string "Hi, there"
    tokenName = GetString();
    tokenType = CONSTANT;
  } // else if
  else if ( gNextChar == '\'' ) {
    tokenName = GetCharConstant( tokenType );
  } // else if
  else if ( IsDigit( gNextChar ) || gNextChar == '.' ) {
    tokenName = GetNum() ;
    if ( tokenName == NULL || strcmp( tokenName, "." ) == 0 ) {
      c[0] = '.';
      c[1] = '\0';
      tokenName = new char[2];
      strcpy( tokenName, c );
      tokenType = UNRECOGNIZE;
      // printf("Unrecognized token with first char : '%c'\n", '.');
    } // if
    else tokenType = CONSTANT;

  } // else if Digit
  else if ( IsOthers( gNextChar ) ) {
    tokenName = GetOthers( tokenType ) ;
  } // else if OTHERS
  else { // Unrecognized token!!!
    c[0] = gNextChar;
    c[1] = '\0';
    tokenName = new char[2];
    strcpy( tokenName, c );
    tokenType = UNRECOGNIZE;
    // printf("Unrecognized token with first char : '%c'\n", gNextChar);
    GetNextChar( gNextChar, gNextCharLine, gNextCharColumn );
  } // else


  return tokenName;

} // GetToken()

void OrderInsertToken( TokenPtr & head, CharPtr tokenStr,
                       TokenType tokenType, int tokenLine, int tokenColumn ) {

  TokenPtr tempt = head;
  TokenPtr temptoken = head;
  TokenPtr pre = NULL;
  ColumnPtr tempc = new Column;
  LinePtr tempL = NULL;
  temptoken = new Token;
  temptoken -> next = NULL;
  temptoken -> tokenStr = tokenStr;
  temptoken -> type = tokenType;

  tempL = new Line;
  tempL -> next = NULL;
  tempL -> line = tokenLine;

  tempc = new Column;
  tempc -> column = tokenColumn;
  tempc -> next = NULL;

  tempL -> firstAppearAt = tempc;
  tempL -> lastAppearAt = tempc;

  temptoken -> firstAppearOn = tempL;
  temptoken -> lastAppearOn = tempL;

  if ( head == NULL ) {
    head = temptoken;
  } // if

  else {
    while ( tempt != NULL ) {
      pre = tempt;
      tempt = tempt -> next;
    } // while

    pre -> next = temptoken;
  } // else

} // OrderInsertToken()

int main() {
  char ch = '\0' ;
  CharPtr tokenStr = NULL ;
  int tokenLine = 0, tokenColumn = 0 ;
  TokenType tokenType ;
  TokenPtr listHead = NULL ;   // !!! empty token list at the start
  scanf( "%d%c", &uTestNum, &ch ) ;
  printf( "Our-C running ...\n" );

  GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
  BuildSpeInput();

  do { // 若定義的不存在要如何保存先peek的token

    if ( gKeepToken == NULL ) {
      tokenStr = GetToken( listHead, tokenType, tokenLine, tokenColumn ) ;
      OrderInsertToken( listHead, tokenStr, tokenType, tokenLine, tokenColumn ) ;
    } // if
    else {
      OrderInsertToken( listHead, gKeepToken, gType, 1, 1 ) ;

      gColumn = 1;
      gKeepToken = NULL;
    } // else

    gNotFinish = false;
    gFirstTokenDone = false;
    // GetTokenInfo( listHead );
    if ( tokenType == UNRECOGNIZE ) { // token unrecognized
      if ( gNextChar != '\n' ) {
        scanf( "%c", &ch );
        while ( ch != '\n' ) scanf( "%c", &ch ); // read char till '\n'
      } // if

      printf( "> Line %d : unrecognized token with first char : \'%s\'\n", tokenLine, tokenStr );
      Reset( listHead );
      gColumn = 1 ;
      GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
    } // if
    else if ( Grammar( listHead ) && !gNotFinish ) {
      // GetTokenInfo( listHead );
      Reset( listHead );
    } // else if
    else if ( gerror ) {
      // GetTokenInfo( listHead );
      gColumn = 1 ;
      Reset( listHead );
      if ( gKeepToken == NULL ) GetNextChar( gNextChar, gNextCharLine, gNextCharColumn ) ;
      // printf( "grammar error\n" );
    } // else if
    else if ( gKeepToken != NULL && gKeep ) { // 處理if-else
      gKeep = false;
      gKeepToken = NULL;
    } // else if

    if ( gDone ) {
      // GetTokenInfo( listHead );
      gFunTable.clear();
      gIDTable.clear();
      printf( "> Our-C exited ..." );
    } // if

  } while ( !gDone ) ;

  return 0 ;

} // main()


void Reset( TokenPtr &head ) {
  DeleteAllTokens( head );
  gLine = 1 ;
  // gColumn = 1 ;
  gNotFinish = false;
  gerror = false;
  gFirstTokenDone = false;
  gPrint = false;
  gKeep = true;
  gLocIDTb.clear();
  // printf( "Reset\n" );

} // Reset()

void BuildSpeInput() {
  string name0 = "cin";
  string name1 = "cout";
  string names[] = {
    "ListAllVariables", "ListAllFunctions",
    "ListVariable", "ListFunction", "Done"
                   };
  ID id1, id2;
  id1.again = false;
  id1.tokenName = name0;
  gIDTable.push_back( id1 );

  id2.again = false;
  id2.tokenName = name1;
  gIDTable.push_back( id2 );

  for ( int i = 0 ; i < 5 ; i++ ) {
    ID fun;
    fun.again = false;
    fun.tokenName = names[i];
    gFunTable.push_back( fun );
  } // for

} // BuildSpeInput()

bool CheckDefineID( TokenPtr head ) {
  for ( int i = 0 ; i < gIDTable.size() ; i ++ ) { // 定義過
    if ( gIDTable[i].tokenName.compare( head -> tokenStr ) == 0 ) return true;
  } // for

  for ( int i = 0 ; i < gLocIDTb.size() ; i ++ ) {
    for ( int j = 0 ; j < gLocIDTb[i].size() ; j ++ ) {
      if ( gLocIDTb[i][j].id.compare( head -> tokenStr ) == 0 ) return true;
    } // for
  } // for

  UndefinedToken( head );
  return false;
} // CheckDefineID()

bool CheckDefineFun( TokenPtr head ) {
  for ( int i = 0 ; i < gFunTable.size() ; i ++ ) { // 定義過
    if ( gFunTable[i].tokenName.compare( head -> tokenStr ) == 0 ) return true;
  } // for

  UndefinedToken( head );
  return false;
} // CheckDefineFun()

string TokenTypeStr( TokenType type ) {
  if ( type == INT ) return "int";
  else if ( type == STRING ) return "string";
  else if ( type == FLOAT ) return "float";
  else if ( type == BOOL ) return "boolean";
  else if ( type == CHAR ) return "char";
  else printf( "no such type\n" );
  return 0;
} // TokenTypeStr()

bool PrintFunction( string funName ) {
  int pos = -1;
  int blank = 0;
  for ( int i = 0 ; i < gFunTable.size() ; i ++ ) {
    if ( gFunTable[i].tokenName.compare( funName ) == 0 ) pos = i;
  } // for

  if ( pos == -1 ) return false; // function not found
  else {
    vector<Fun> fun = gFunTable[pos].content;
    for ( int i = 0 ; i < fun.size() ; i ++ ) {
      printf( "%s", fun[i].token.c_str() );
      // ID後面如是 ( or [ or PP or MM不用空格 ex. a++
      if ( i != fun.size() - 1 && fun[i].type == IDENTIFIER &&
           ( fun[i+1].token.compare( "(" ) == 0 || fun[i+1].token.compare( "[" ) == 0
             || fun[i+1].type == PP || fun[i+1].type == MM ) ) ;
             // ++i --i
      else if ( i != fun.size() - 1 && ( fun[i].type == PP || fun[i].type == MM )
                && fun[i+1].type == IDENTIFIER ) ;
      else if ( i != fun.size() - 1 && fun[i].token.compare( "(" ) == 0 &&
                fun[i+1].token.compare( ")" ) == 0 ) ;
      else if ( i != fun.size() - 1 && fun[i + 1].token.compare( "," ) == 0 ) ;

      else if ( fun[i].token.compare( ";" ) != 0 &&
                fun[i].token.compare( "}" ) != 0 &&
                fun[i].token.compare( "{" ) != 0 ) printf( " " );

      if ( fun[i].token.compare( "{" ) == 0 ) blank += 2; // 縮排

      if ( fun[i].token.compare( "{" ) == 0 || fun[i].token.compare( "}" ) == 0
           || fun[i].token.compare( ";" ) == 0 ||
           ( i != fun.size() - 1 &&
             fun[i].type == DO && fun[i+1].token.compare( "{" ) != 0 ) ) {
        printf( "\n" ); // { or } or ; 換行

        // 出 while or if
        if ( blank != 0 && i != fun.size() - 1 &&
             fun[i+1].token.compare( "}" ) == 0 ) blank -= 2;

        if ( i != fun.size() - 1 && fun[i].type == DO &&
             fun[i+1].token.compare( "{" ) != 0 ) blank += 2;
        for ( int i = 0 ; i < blank ; i ++ ) {
          printf( " " ); // 印縮排
        } // for

        if ( i != fun.size() - 1 && fun[i].type == DO &&
             fun[i+1].token.compare( "{" ) != 0 ) blank -= 2;
      } // if
    } // for
  } // else

  return true;
} // PrintFunction()

bool ExecuteFun( TokenPtr Funhead ) {
  string str;
  string names[] = {
    "ListAllVariables", "ListAllFunctions",
    "ListVariable", "ListFunction", "Done"
                   };
  if ( names[4].compare( Funhead -> tokenStr ) == 0 ) {
    if ( Funhead -> next -> next != NULL ) Funhead = Funhead -> next -> next;

    if ( strcmp( Funhead -> tokenStr, ")" ) == 0 ) {
      gDone = true;
      return false; // Done() 不輸出statement execute
    } // if

    return true;
  } // if
  else if ( names[0].compare( Funhead -> tokenStr ) == 0 ) {
    // sort( gIDTable.begin(), gIDTable.end(), SortID );
    for ( int i = 0 ; i < gIDTable.size() ; i++ ) { // sort
      for ( int j = i + 1 ; j < gIDTable.size() ; j++ ) {
        if ( gIDTable[i].tokenName > gIDTable[j].tokenName ) {
          ID temp = gIDTable[i];
          gIDTable[i] = gIDTable[j];
          gIDTable[j] = temp;
        } // if
      } // for
    } // for

    if ( !gPrint ) {
      printf( "> " );
      gPrint = true;
    } // if

    for ( int i = 0 ; i < gIDTable.size() ; i ++ ) {
      if ( gIDTable[i].tokenName.compare( "cin" ) != 0 && gIDTable[i].tokenName.compare( "cout" ) != 0 )
        printf( "%s\n", gIDTable[i].tokenName.c_str() );
    } // for
  } // else if
  else if ( names[1].compare( Funhead -> tokenStr ) == 0 ) { // sort
    for ( int i = 0 ; i < gFunTable.size() ; i++ ) {
      for ( int j = i + 1 ; j < gFunTable.size() ; j++ ) {
        if ( gFunTable[i].tokenName > gFunTable[j].tokenName ) {
          ID temp = gFunTable[i];
          gFunTable[i] = gFunTable[j];
          gFunTable[j] = temp;
        } // if
      } // for
    } // for

    if ( !gPrint ) {
      printf( "> " );
      gPrint = true;
    } // if

    for ( int i = 0 ; i < gFunTable.size() ; i ++ ) {
      if ( gFunTable[i].tokenName != names[0] &&
           gFunTable[i].tokenName != names[1] &&
           gFunTable[i].tokenName != names[2] &&
           gFunTable[i].tokenName != names[3] &&
           gFunTable[i].tokenName != names[4]
         ) {
        printf( "%s()\n", gFunTable[i].tokenName.c_str() );
      } // if
    } // for
  } // else if
  else if ( names[2].compare( Funhead -> tokenStr ) == 0 ) { // ListVariable
    if ( strcmp( Funhead -> next -> next -> tokenStr, ")" ) != 0 ) { // 代表不是ListVariable() 括號內是空的
      str = Funhead -> next -> next -> tokenStr;
      if ( ( str[0] == '"' && str[str.size() - 1] == '"' ) ||
           ( str[0] == '\'' && str[str.size() - 1] == '\'' ) ) {
        str = str.substr( 1, str.size() - 2 );
      } // if
    } // if

    if ( !gPrint ) {
      printf( "> " );
      gPrint = true;
    } // if

    for ( int i = 0 ; i < gIDTable.size() ; i ++ ) { // 定義過
      if ( gIDTable[i].tokenName.compare( str ) == 0 ) {
        printf( "%s %s", TokenTypeStr( gIDTable[i].type ).c_str(), str.c_str() );
        if ( gIDTable[i].arr != "-1" ) printf( "[ %s ]", gIDTable[i].arr.c_str() );
        printf( " ;\n" );
        return true;
      } // if
    } // for

    printf( "Error do not have this variable!!!" );
    return true;
  } // else if
  else if ( names[3].compare( Funhead -> tokenStr ) == 0 ) {
    if ( strcmp( Funhead -> next -> next -> tokenStr, ")" ) != 0 ) {
      str = Funhead -> next -> next -> tokenStr;
      if ( ( str[0] == '"' && str[str.size() - 1] == '"' ) ||
           ( str[0] == '\'' && str[str.size() - 1] == '\'' ) ) {
        str = str.substr( 1, str.size() - 2 );
      } // if
    } // if

    if ( !gPrint ) {
      printf( "> " );
      gPrint = true;
    } // if

    if ( PrintFunction( str ) ) return true;
    else printf( "Error do not have this function!!!" );
    return false ;

  } // else if

  return true;
} // ExecuteFun()

void BuildGloID( TokenPtr head, vector<ArrayID> gloIDvec ) {
  bool define = false;
  for ( int j = 0 ; j < gloIDvec.size() ; j++ ) {
    define = false;
    for ( int i = 0 ; i < gIDTable.size() && !define ; i ++ ) { // 定義過
      if ( gIDTable[i].tokenName == gloIDvec[j].id ) {
        gIDTable[i].again = true;
        gIDTable[i].type = head -> type;
        if ( gloIDvec[j].arr != "-1" ) gIDTable[i].arr = gloIDvec[j].arr;
        define = true;

        if ( !gPrint ) {
          printf( "> " );
          gPrint = true;
        } // if

        printf( "New definition of %s entered ...\n", gloIDvec[j].id.c_str() );
      } // if
    } // for

    if ( !define ) {
      ID id;
      id.again = false;
      id.tokenName = gloIDvec[j].id ;
      id.type = head -> type;
      id.arr = gloIDvec[j].arr;
      if ( gloIDvec[j].arr != "-1" ) id.arr = gloIDvec[j].arr;
      gIDTable.push_back( id );
      if ( !gPrint ) {
        printf( "> " );
        gPrint = true;
      } // if

      printf( "Definition of %s entered ...\n", gloIDvec[j].id.c_str() );

    } // if
  } // for
} // BuildGloID()

void BuildGloFun( TokenPtr head, CharPtr FunName ) {
  ID id;
  for ( int i = 0 ; i < gFunTable.size() ; i ++ ) { // 定義過
    if ( gFunTable[i].tokenName.compare( FunName ) == 0 ) {
      printf( "> New definition of %s() entered ...\n", FunName );
      gFunTable[i].again = true;
      gFunTable[i].content.clear(); // clear original content
      while ( head != NULL ) { // put new content
        Fun fun;
        fun.token = head -> tokenStr;
        fun.type = head -> type;
        gFunTable[i].content.push_back( fun ); // function content
        head = head -> next;
      } // while

      return;
    } // if
  } // for

  id.again = false;
  id.tokenName = FunName;
  printf( "> Definition of %s() entered ...\n", FunName );

  while ( head != NULL ) {
    Fun fun;
    fun.token = head -> tokenStr;
    fun.type = head -> type;
    id.content.push_back( fun ); // function content
    head = head -> next;
  } // while

  gFunTable.push_back( id );
} // BuildGloFun()

bool Grammar( TokenPtr listHead ) {
  TokenPtr head = listHead;
  CharPtr peekToken = NULL;
  if ( head -> type == QUIT ) return true;
  else if ( User_input( head ) ) return true;

  return false;
} // Grammar()

bool CheckConstant( TokenPtr &head ) { // [ '[' Constant ']' ]
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( strcmp( peekToken -> tokenStr, "[" ) == 0 ) {
    if ( !GetNextToken( head ) )  return false; // get [
    else if ( !GetNextToken( head ) )  return false;
    else if ( head -> type != CONSTANT ) {
      UnexpToken( head );
      return false;
    } // else if
    else {
      if ( !GetNextToken( head ) )  return false;
      else if ( strcmp( head -> tokenStr, "]" ) != 0 ) {
        UnexpToken( head );
        return false;
      } // else if
    } // else
  } // if

  return false;
} // CheckConstant()

bool User_input( TokenPtr &head ) {
  if ( Definition( head ) ) return true;
  else if ( Statement( head ) && !gerror ) {
    if ( !gPrint ) printf( "> " );
    printf( "Statement executed ...\n" );
    return true;
  } // else if
  else { // 兩個都沒做
    if ( !gerror && !gNotFinish && !gDone ) UnexpToken( head );
    gLocIDTb.clear();
    return false;
  } // else

  return false;
} // User_input()

bool Definition( TokenPtr &head ) {
  // VOID Identifier function_definition_without_ID
  TokenPtr headForDefine = head;
  CharPtr funName;
  CharPtr name;
  CharPtr curID;
  int num = 0;
  vector<ArrayID> gloIDvec;
  ArrayID arrID;
  if ( head -> type == VOID ) {
    gFirstTokenDone = true;
    if ( !GetNextToken( head ) )  return false; // get id
    else if ( head -> type == IDENTIFIER ) {
      funName = head -> tokenStr;
      if ( Function_definition_without_ID( head ) ) {
        // 確定輸入正確才存入gFunTable
        BuildGloFun( headForDefine, funName );
        return true;
      } // if
      else return false;
    } // else if
    else { // not id
      UnexpToken( head );
      return false;
    } // else
  } // if
  else if ( Type_specifier( head ) ) {
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false; // get typeSpecifier
    } // if
    else {
      gFirstTokenDone = true;
    } // else

    if ( !GetNextToken( head ) )  return false; // get id
    else if ( head -> type == IDENTIFIER ) {
      name = head -> tokenStr;
      if ( Rest_of_declarators( head, gloIDvec ) ) {
        // 確定輸入正確才存入gIDTable
        BuildGloID( headForDefine, gloIDvec );
        gloIDvec.clear();
        return true;
      } // if
      else if ( Function_definition_without_ID( head ) ) {
        BuildGloFun( headForDefine, name );
        return true;
      } // else if
      else {
        gloIDvec.clear();
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else
    } // else if
    else {
      UnexpToken( head );
      return false;
    } // else
  } // else if
  else return false;
} // Definition()

bool Type_specifier( TokenPtr &head ) {
  // INT | CHAR | FLOAT | STRING | BOOL
  if ( head -> type == INT || head -> type == FLOAT || head -> type == STRING ||
       head -> type == CHAR || head -> type == BOOL ) {
    return true;
  } // if

  return false;
} // Type_specifier()

// bool Function_definition_or_declarators( TokenPtr &head ) {
  //  : function_definition_without_ID
  // | rest_of_declarators
  // if ( Rest_of_declarators( head ) ) return true;
  // else if ( Function_definition_without_ID( head ) ) return true;
  // return false;
// } // Function_definition_or_declarators()

bool Rest_of_declarators( TokenPtr &head, vector<ArrayID> &IDvec ) {
  // [ '[' Constant ']' ]
  // { ',' Identifier [ '[' Constant ']' ] } ';'
  ArrayID arrID;
  bool inFirstIf = false;
  bool inSecondIf = false;
  TokenPtr peekToken = NULL;
  arrID.id = head -> tokenStr;
  TokenPtr temp = head;
  if ( !CheckConstant( head ) && ( gerror || gNotFinish )  ) return false; // [ '[' Constant ']' ]
  else if ( strcmp( temp -> next -> tokenStr, "[" ) == 0 ) {
    arrID.arr = temp -> next -> next -> tokenStr; // 存array 大小
  } // else if
  else arrID.arr = "-1";

  IDvec.push_back( arrID );

  if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "," ) == 0  ) {
    inSecondIf = true;
    while ( strcmp( peekToken -> tokenStr, "," ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token ","
      else if ( !GetNextToken( head ) )  return false; // get next token
      else if ( head -> type != IDENTIFIER ) {
        UnexpToken( head );
        return false;
      } // else if
      else {
        arrID.id = head -> tokenStr;
      } // else

      temp = head;
      if ( !CheckConstant( head ) && ( gerror || gNotFinish )  ) return false; // [ '[' Constant ']' ]
      else if ( strcmp( temp -> next -> tokenStr, "[" ) == 0 ) {
        arrID.arr = temp -> next -> next -> tokenStr; // 存array 大小
      } // else if
      else arrID.arr = "-1";

      IDvec.push_back( arrID );
      if ( !PeekNextToken( head, peekToken ) )  return false;

    } // while
  } // else if

  if ( !inFirstIf && !inSecondIf ) {
    if ( strcmp( peekToken -> tokenStr, ";" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get ;
      return true;
    } // if
    else {
      return false; // run function_definition_without_ID
    } // else
  } // if
  else {
    if ( !GetNextToken( head ) )  return false;
    else if ( strcmp( head -> tokenStr, ";" ) == 0 ) return true;
    else {
      UnexpToken( head );
      return false;
    } // else
  } // else

} // Rest_of_declarators()

bool Function_definition_without_ID( TokenPtr &head ) {
  // '(' [ VOID | formal_parameter_list ] ')' compound_statement
  TokenPtr peekToken = NULL;

  if ( !GetNextToken( head ) ) return false;
  if ( strcmp( head -> tokenStr, "(" ) != 0 ) {
    UnexpToken( head );
    return false;
  } // if
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == VOID ) {
    if ( !GetNextToken( head ) )  return false; // get token "void"
  } // else if
  else if ( !Formal_parameter_list( head ) && ( gerror || gNotFinish ) ) return false;

  if ( !GetNextToken( head ) )  return false; // get token
  else if ( strcmp( head -> tokenStr, ")" ) != 0 ) {
    UnexpToken( head );
    return false;
  } // else if

  if ( !Compound_statement( head ) ) {
    if ( !gerror && !gNotFinish ) UnexpToken( head ); // 一定要做
    return false;
  } // if
  else return true;
} // Function_definition_without_ID()

bool Formal_parameter_list( TokenPtr &head ) {
  // type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
  // { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  vector<ArrayID> readLocalID;
  ArrayID arrID;
  TokenPtr peekToken = NULL;
  CharPtr id ;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( Type_specifier( peekToken ) ) { // Type_specifier [ '&' ] Identifier
    if ( !GetNextToken( head ) )  return false; // get Type_specifier token
    else if ( !PeekNextToken( head, peekToken ) )  return false;
    else if ( strcmp( peekToken -> tokenStr,  "&" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token "&"
    } // else if

    if ( !GetNextToken( head ) )  return false; // get identifier
    else if ( head -> type != IDENTIFIER ) {
      UnexpToken( head );
      return false;
    } // else if
    else {
      arrID.id = head -> tokenStr;
      readLocalID.push_back( arrID );
    } // else

    if ( !CheckConstant( head ) && ( gerror || gNotFinish ) ) return false; // [ '[' Constant ']' ]
    // { ',' Type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
    if ( !PeekNextToken( head, peekToken ) )  return false;
    while ( strcmp( peekToken -> tokenStr, "," ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get ","
      else if ( !GetNextToken( head ) )  return false;
      else if ( !Type_specifier( head ) ) {
        UnexpToken( head );
        return false;
      } // else if
      else {
        if ( !GetNextToken( head ) )  return false;
        else if ( strcmp( head -> tokenStr, "&" ) == 0 ) {
          if ( !GetNextToken( head ) )  return false;
        } // else if

        if ( head -> type != IDENTIFIER ) {
          UnexpToken( head );
          return false;
        } // else if
        else { // [ '[' Constant ']' ]
          arrID.id = head -> tokenStr;
          readLocalID.push_back( arrID );
          if ( !CheckConstant( head ) && ( gerror || gNotFinish ) ) return false; // [ '[' Constant ']' ]
        } // else
      } // else

      if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    gLocIDTb.push_back( readLocalID ); // parameter id宣告
    readLocalID.clear();
    return true;
  } // if

  return false;
} // Formal_parameter_list()

bool Compound_statement( TokenPtr &head ) { // 可能沒做，一定要做的話要檢查gerror
  // '{' { declaration | statement } '}'
  vector<ArrayID> readLocalID;
  bool keep = true;
  int start = 0;
  int num = 0; // count 每次進compound statement區域變數加入vector數量
  TokenPtr peekToken = NULL;
  if ( !gFirstTokenDone ) peekToken = head;  // first token already get
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( strcmp( peekToken -> tokenStr, "{" ) == 0 ) {

    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false; // get token "{"
    } // if
    else gFirstTokenDone = true;
    while ( keep ) {
      keep = false;
      readLocalID.clear();
      if ( Declaration( head, readLocalID ) ) {
        keep = true;
        gLocIDTb.push_back( readLocalID );
        num ++;
      } // if
      else if ( Statement( head ) ) {
        keep = true;
      } // else if
      else if ( gerror ) return false;
    } // while

    if ( !GetNextToken( head ) )  return false; // get token
    else if ( strcmp( head -> tokenStr, "}" ) != 0 ) {
      UnexpToken( head );
      return false;
    } // else if
    else { // pop num 數量from vector 一個{}結束 pop local variable
      start = gLocIDTb.size() - num;
      if ( gLocIDTb.size() != 0 ) gLocIDTb.erase( gLocIDTb.begin() + start, gLocIDTb.end() );
      return true;
    } // else
  } // if
  else return false;
} // Compound_statement()

bool Declaration( TokenPtr &head, vector<ArrayID> &local ) {
  // function id define
  CharPtr newName ;
  TokenPtr peekToken = NULL;
  TokenPtr headForDefine = head;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( !Type_specifier( peekToken ) ) return false;
  else {
    if ( !GetNextToken( head ) )  return false; // Type_specifier
    else if ( !GetNextToken( head ) )  return false; // get id

    if ( head -> type != IDENTIFIER ) {
      UnexpToken( head );
      return false;
    } // if
    else {
      newName = new char[ strlen( head -> tokenStr ) + 2];
      strcpy( newName, head -> tokenStr );
      if ( !Rest_of_declarators( head, local ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // if
      else {
        // local.push_back( newName );
        return true;
      } // else
    } // else
  } // else

} // Declaration()

bool Statement( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !gFirstTokenDone ) peekToken = head; // first token already get
  else if ( !PeekNextToken( head, peekToken ) )  return false;

  if ( strcmp( peekToken -> tokenStr, ";" ) == 0 ) {
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false;
    } // if
    else gFirstTokenDone = true;

    return true;
  } // if
  else if ( peekToken -> type == RETURN ) {
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false; // get token "return"
    } // if
    else gFirstTokenDone = true;
    // 若gerror = false 代表沒有執行Expression
    if ( !Expression( head ) && ( gerror || gNotFinish ) ) return false;
    else if ( !GetNextToken( head ) )  return false; // get next token
    else if ( strcmp( head -> tokenStr, ";" ) == 0 ) return true;
    else {
      UnexpToken( head );
      return false;
    } // else
  } // else if
  else if ( peekToken -> type == IF ) {
    // IF '(' expression ')' statement [ ELSE statement ]
    // get token "if"
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false;
    } // if
    else gFirstTokenDone = true;

    if ( !CheckExpression( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // if
    else if ( !Statement( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // else if
    else if ( !PeekNextToken( head, peekToken ) )  return false;
    else if ( peekToken -> type == ELSE ) {
      if ( !GetNextToken( head ) )  return false; // get "else"
      else if ( !Statement( head ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else return true;
    } // else if
    else { // 若沒有else 要keep peek的token
      if ( gKeep ) {
        gKeepToken = new char[30];
        strcpy( gKeepToken, peekToken -> tokenStr );
        gType = peekToken -> type;
      } // if

      return true;
    } // else
  } // else if
  else if ( peekToken -> type == WHILE ) {
    // WHILE '(' expression ')' statement
    // get token "while"
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false; // get while
    } // if
    else gFirstTokenDone = true;

    if ( !CheckExpression( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // if
    else if ( !Statement( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // else if
    else return true;
  } // else if
  else if ( peekToken -> type == DO ) {
    // DO statement WHILE '(' expression ')' ';'
    if ( gFirstTokenDone ) { // get token "do"
      if ( !GetNextToken( head ) )  return false;
    } // if
    else gFirstTokenDone = true;

    if ( !Statement( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // if
    else if ( !GetNextToken( head ) )  return false;
    else if ( head -> type != WHILE ) {
      UnexpToken( head );
      return false;
    } // else if
    else if ( !CheckExpression( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // else if
    else if ( !GetNextToken( head ) )  return false;
    else if ( strcmp( head -> tokenStr, ";" ) == 0  ) return true;
    else {
      UnexpToken( head );
      return false;
    } // else

    return true;
  } // else if
  else if ( Compound_statement( head )  ) return true;
  else if ( Expression( head ) ) { // Expression 一定要做，false gerror have to be true
    if ( !GetNextToken( head ) )  return false;
    else if ( strcmp( head -> tokenStr, ";" ) == 0 ) return true;
    else {
      UnexpToken( head );
      return false;
    } // else
  } // else if
  else return false;

} // Statement()

bool Expression( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !Basic_expression( head ) ) return false; // have to get token
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "," ) == 0 ) {
    while (  strcmp( peekToken -> tokenStr, "," ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token ","
      else if ( !Basic_expression( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Expression()

bool Basic_expression( TokenPtr &head ) { // 如一定要做，return false 要檢查gerror是否為true
  TokenPtr peekToken = NULL;
  if ( !gFirstTokenDone ) peekToken = head; // first token already get
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( peekToken -> type == IDENTIFIER ) {
    // Identifier rest_of_Identifier_started_basic_exp
    if ( gFirstTokenDone ) { // not id start
      if ( !GetNextToken( head ) )  return false; // get id
    } // if
    else gFirstTokenDone = true;

    if ( !Rest_of_Identifier_started_basic_exp( head ) ) return false;
    else return true;
  } // if
  else if ( peekToken -> type == PP || peekToken -> type == MM ) {
    // ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false; // pp mm
    } // if
    else gFirstTokenDone = true;

    if ( !GetNextToken( head ) )  return false; // get token
    else if ( head -> type != IDENTIFIER ) {
      UnexpToken( head );
      return false;
    } // else if

    else if ( !Rest_of_PPMM_Identifier_started_basic_exp( head ) ) return false;
    else return true;
  } // else if
  else if ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ||
            strcmp( peekToken -> tokenStr, "!" ) == 0 ) {
    // sign { sign } signed_unary_exp romce_and_romloe
    while ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ||
            strcmp( peekToken -> tokenStr, "!" ) == 0 ) {
      // get sign token
      if ( gFirstTokenDone ) {
        if ( !GetNextToken( head ) )  return false;
      } // if
      else gFirstTokenDone = true;

      if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    if ( !Signed_unary_exp( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // if
    else if ( !Romce_and_romloe( head ) ) return false;
    else return true;
  } // else if
  else if ( peekToken -> type == CONSTANT || strcmp( peekToken -> tokenStr, "(" ) == 0 ) {
  // ( Constant | '(' expression ')' ) romce_and_romloe
    if ( strcmp( peekToken -> tokenStr, "(" ) == 0 ) {
      if ( !CheckExpression( head ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // if
    } // if
    else if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false; // get token Constant
    } // else if
    else gFirstTokenDone = true;

    if ( !Romce_and_romloe( head ) ) return false;
    else return true;
  } // else if
  else {
    // UnexpToken( head );
    return false;
  } // else

} // Basic_expression()

bool Rest_of_Identifier_started_basic_exp( TokenPtr &head ) {
  // : [ '[' expression ']' ] ( assignment_operator basic_expression
  // | [ PP | MM ] romce_and_romloe )  |
  // '(' [ actual_parameter_list ] ')' romce_and_romloe
  TokenPtr temp = head; // remember head
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) ) return false;
  else if ( strcmp( peekToken -> tokenStr, "(" ) == 0 ) { // call function
    // '(' [ actual_parameter_list ] ')' romce_and_romloe
    if ( !CheckDefineFun( head ) ) return false;
    if ( !GetNextToken( head ) )  return false; // get token "("
    // 若!gerror 代表沒有執行Actual_parameter_list
    else if ( !Actual_parameter_list( head ) && ( gerror || gNotFinish ) ) return false;
    else if ( !GetNextToken( head ) )  return false; // get token
    else if ( strcmp( head -> tokenStr, ")" ) == 0 ) {
      if ( !Romce_and_romloe( head ) ) return false;
      else {
        if ( !PeekNextToken( head, peekToken ) ) return false; // 先確認下一個token是";"才執行
        else if ( strcmp( peekToken -> tokenStr, ";" ) == 0 ) { // function 內call function 會重複執行
          if ( !ExecuteFun( temp ) ) return false; // Done();
          else return true;
        } // else if
        else return true;
      } // else
    } // else if
    else {
      UnexpToken( head );
      return false;
    } // else
  } // else if
  else { // call id
    // [ '[' expression ']' ]
    if ( !CheckDefineID( head ) ) return false; // check if id define
    else if ( strcmp( peekToken -> tokenStr, "[" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token "["
      else if ( !Expression( head ) ) { // Expression 一定要做，false gerror have to be true
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // if
      else if ( !GetNextToken( head ) )  return false; // get token
      else if ( strcmp( head -> tokenStr, "]" ) != 0 ) {
        UnexpToken( head );
        return false;
      } // else if
    } // else if

    // ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
    if ( !PeekNextToken( head, peekToken ) )  return false;
    // assignment_operator basic_expression
    else if ( Assignment_operator( head ) ) {
      if ( !GetNextToken( head ) )  return false; // get Assignment_operator
      if ( !Basic_expression( head ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else return true;
    } // else if

    // [ PP | MM ] romce_and_romloe
    else if ( peekToken -> type == PP || peekToken -> type == MM ) {
      if ( !GetNextToken( head ) )  return false; // get PP || MM
      else if ( !Romce_and_romloe( head ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else return true;
    } // else if
    else if ( Romce_and_romloe( head ) ) return true;
    else return false;
  } // else

  return false;
} // Rest_of_Identifier_started_basic_exp()

bool Rest_of_PPMM_Identifier_started_basic_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "[" ) == 0 ) {
    if ( !CheckDefineID( head ) ) return false; // check if id define
    else if ( !GetNextToken( head ) )  return false; // get "["
    else if ( !Expression( head ) ) { // Expression 一定要做，false gerror have to be true
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // else if
    else if ( !GetNextToken( head ) )  return false; // get next token
    else if ( strcmp( head -> tokenStr, "]" ) != 0 ) {
      UnexpToken( head );
      return false;
    } // if
    else if ( !Romce_and_romloe( head ) ) return false;
    else return true;
  } // else if
  else if ( !Romce_and_romloe( head ) ) return false;
  else return true;

} // Rest_of_PPMM_Identifier_started_basic_exp()

bool Actual_parameter_list( TokenPtr &head ) {
  // Actual_parameter_list包在[]裡，可能不做
  TokenPtr peekToken = NULL;
  if ( !Basic_expression( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "," ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "," ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token ","
      if ( !Basic_expression( head ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // if
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Actual_parameter_list()


bool Assignment_operator( TokenPtr &head ) { // 若都不是直接unexpected token
  // '=' | TE | DE | RE | PE | ME
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "=" ) == 0 || peekToken -> type == TE ||
            peekToken -> type == DE || peekToken -> type == RE ||
            peekToken -> type == PE || peekToken -> type == ME ) {
    return true;
  }  // else if
  else return false;
} // Assignment_operator()

bool Romce_and_romloe( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !Rest_of_maybe_logical_OR_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "?" ) == 0 ) {
    if ( !GetNextToken( head ) )  return false; // get token "?"
    else if ( !Basic_expression( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // else if
    else if ( !GetNextToken( head ) )  return false; // get ":"
    else if ( strcmp( head -> tokenStr, ":" ) != 0 ) {
      UnexpToken( head );
      return false;
    } // else if
    else if ( !Basic_expression( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // else if
    else return true;
  } // else if
  else return true;

} // Romce_and_romloe()

bool Rest_of_maybe_logical_OR_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_logical_AND_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == OR ) {
    while ( peekToken -> type == OR ) {
      if ( !GetNextToken( head ) )  return false; // get token OR
      else if ( !Maybe_logical_AND_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Rest_of_maybe_logical_OR_exp()

bool Maybe_logical_AND_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_bit_OR_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == AND ) {
    while ( peekToken -> type == AND ) {
      if ( !GetNextToken( head ) )  return false; // get token AND
      else if ( !Maybe_bit_OR_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Maybe_logical_AND_exp()

bool Rest_of_maybe_logical_AND_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_bit_OR_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == AND ) {
    while ( peekToken -> type == AND ) {
      if ( !GetNextToken( head ) )  return false; // get token AND
      else if ( !Maybe_bit_OR_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Rest_of_maybe_logical_AND_exp()

bool Maybe_bit_OR_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_bit_ex_OR_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "|" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "|" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token |
      else if ( !Maybe_bit_ex_OR_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Maybe_bit_OR_exp()

bool Rest_of_maybe_bit_OR_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_bit_ex_OR_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "|" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "|" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token |
      else if ( !Maybe_bit_ex_OR_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Rest_of_maybe_bit_OR_exp()

bool Maybe_bit_ex_OR_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_bit_AND_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "^" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "^" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token ^
      else if ( !Maybe_bit_AND_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Maybe_bit_ex_OR_exp()

bool Rest_of_maybe_bit_ex_OR_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_bit_AND_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "^" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "^" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token ^
      else if ( !Maybe_bit_AND_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Rest_of_maybe_bit_ex_OR_exp()

bool Maybe_bit_AND_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_equality_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "&" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "&" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token &
      else if ( !Maybe_equality_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Maybe_bit_AND_exp()

bool Rest_of_maybe_bit_AND_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_equality_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "&" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "&" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token &
      else if ( !Maybe_equality_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;
} // Rest_of_maybe_bit_AND_exp()

bool Maybe_equality_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_relational_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == EQ || peekToken -> type == NEQ ) {
    while ( peekToken -> type == EQ || peekToken -> type == NEQ ) {
      if ( !GetNextToken( head ) )  return false; // get token EQ or NEQ
      else if ( !Maybe_relational_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Maybe_equality_exp()

bool Rest_of_maybe_equality_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_relational_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == EQ || peekToken -> type == NEQ ) {
    while ( peekToken -> type == EQ || peekToken -> type == NEQ ) {
      if ( !GetNextToken( head ) )  return false; // get token EQ or NEQ
      else if ( !Maybe_relational_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Rest_of_maybe_equality_exp()

bool Maybe_relational_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_shift_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "<" ) == 0 || strcmp( peekToken -> tokenStr, ">" ) == 0
            || peekToken -> type == LE || peekToken -> type == GE ) {
    while ( strcmp( peekToken -> tokenStr, "<" ) == 0 || strcmp( peekToken -> tokenStr, ">" ) == 0
            || peekToken -> type == LE || peekToken -> type == GE ) {
      if ( !GetNextToken( head ) )  return false; // get token < > LE GE
      else if ( !Maybe_shift_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Maybe_relational_exp()

bool Rest_of_maybe_relational_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_shift_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "<" ) == 0 || strcmp( peekToken -> tokenStr, ">" ) == 0
            || peekToken -> type == LE || peekToken -> type == GE ) {
    while ( strcmp( peekToken -> tokenStr, "<" ) == 0 || strcmp( peekToken -> tokenStr, ">" ) == 0
            || peekToken -> type == LE || peekToken -> type == GE ) {
      if ( !GetNextToken( head ) )  return false; // get token < > LE GE
      else if ( !Maybe_shift_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Rest_of_maybe_relational_exp()

bool Maybe_shift_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_additive_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == LS || peekToken -> type == RS ) {
    while ( peekToken -> type == LS || peekToken -> type == RS ) {
      if ( !GetNextToken( head ) )  return false; // get token LS RS
      else if ( !Maybe_additive_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Maybe_shift_exp()

bool Rest_of_maybe_shift_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_additive_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == LS || peekToken -> type == RS ) {
    while ( peekToken -> type == LS || peekToken -> type == RS ) {
      if ( !GetNextToken( head ) )  return false; // get token LS RS
      else if ( !Maybe_additive_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Rest_of_maybe_shift_exp()

bool Maybe_additive_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Maybe_mult_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token + -
      else if ( !Maybe_mult_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Maybe_additive_exp()


bool Rest_of_maybe_additive_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;

  if ( !Rest_of_maybe_mult_exp( head ) ) return false;
  else if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token + -
      else if ( !Maybe_mult_exp( head ) ) return false;
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // else if
  else return true;

} // Rest_of_maybe_additive_exp()

bool Maybe_mult_exp( TokenPtr &head ) {
  if ( !Unary_exp( head ) ) {
    if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
    return false;
  } // if
  else if ( !Rest_of_maybe_mult_exp( head ) ) return false;
  else return true;
} // Maybe_mult_exp()

bool Rest_of_maybe_mult_exp( TokenPtr &head ) {
  // could be empty !
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( strcmp( peekToken -> tokenStr, "*" ) == 0 || strcmp( peekToken -> tokenStr, "/" ) == 0
       || strcmp( peekToken -> tokenStr, "%" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "*" ) == 0 || strcmp( peekToken -> tokenStr, "/" ) == 0
            || strcmp( peekToken -> tokenStr, "%" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get token * % /
      else if ( !Unary_exp( head ) ) {
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else if ( !PeekNextToken( head, peekToken ) )  return false;
    } // while

    return true;
  } // if
  else return true;

} // Rest_of_maybe_mult_exp()

bool Unary_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  if ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ||
       strcmp( peekToken -> tokenStr, "!" ) == 0 ) {
    while ( strcmp( peekToken -> tokenStr, "+" ) == 0 || strcmp( peekToken -> tokenStr, "-" ) == 0 ||
            strcmp( peekToken -> tokenStr, "!" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false;
      if ( !PeekNextToken( head, peekToken ) )  return false;

    } // while

    if ( !Signed_unary_exp( head ) ) {
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // if
    else return true;
  } // if
  else if ( peekToken -> type == PP || peekToken -> type == MM || peekToken -> type == IDENTIFIER ) {
    if ( peekToken -> type == PP || peekToken -> type == MM ) {
      if ( !GetNextToken( head ) )  return false; // get pp or mm
    } // if

    if ( !GetNextToken( head ) )  return false;
    else if ( head -> type != IDENTIFIER ) {
      UnexpToken( head );
      return false;
    } // else if
    else if ( !CheckDefineID( head ) ) return false; // check id define
    else if ( !PeekNextToken( head, peekToken ) )  return false;
    else if ( strcmp( peekToken -> tokenStr, "[" ) == 0 ) {
      if ( !GetNextToken( head ) )  return false; // get "["
      else if ( !Expression( head ) ) { // Expression 一定要做，false gerror have to be true
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else if ( !GetNextToken( head ) )  return false; // get next token
      else if ( strcmp( head -> tokenStr, "]" ) != 0 ) {
        UnexpToken( head );
        return false;
      } // else if
      else return true;
    } // else if
    else return true;
  } // else if
  else if ( Unsigned_unary_exp( head ) ) return true;
  else return false;
} // Unary_exp()



bool Signed_unary_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == IDENTIFIER ) {
    if ( !GetNextToken( head ) )  return false; // get token id
    else if ( !PeekNextToken( head, peekToken ) )  return false;
    else if ( strcmp( peekToken -> tokenStr, "(" ) == 0 ) {
      // '(' [ actual_parameter_list ] ')'
      if ( !CheckDefineFun( head ) ) return false;
      else if ( !GetNextToken( head ) )  return false; // get token "("
      // 若gerror = false 代表沒有執行Actual_parameter_list
      if ( !Actual_parameter_list( head ) && ( gerror || gNotFinish ) ) return false;
      else if ( !GetNextToken( head ) )  return false; // get token
      else if ( strcmp( head -> tokenStr, ")" ) == 0 ) return true;
      else {
        UnexpToken( head );
        return false;
      } // else
    } // else if
    else if ( strcmp( peekToken -> tokenStr, "[" ) == 0 ) {
      if ( !CheckDefineID( head ) ) return false;
      else if ( !GetNextToken( head ) )  return false; // get "["
      else if ( !Expression( head ) ) { // Expression 一定要做，false gerror have to be true
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else if ( !GetNextToken( head ) )  return false; // get next token
      else if ( strcmp( head -> tokenStr, "]" ) != 0 ) {
        UnexpToken( head );
        return false;
      } // else if
      else return true;
    } // else if
    else {
      if ( !CheckDefineID( head ) ) return false;
      return true;
    } // else
  } // else if
  else if ( peekToken -> type == CONSTANT ) {
    if ( !GetNextToken( head ) )  return false; // get token Constant
    else return true;
  } // else if
  else if ( CheckExpression( head ) ) return true; // 若不是括號unexpected token
  else return false;

} // Signed_unary_exp()

bool Unsigned_unary_exp( TokenPtr &head ) {
  TokenPtr peekToken = NULL;
  if ( !PeekNextToken( head, peekToken ) )  return false;
  else if ( peekToken -> type == IDENTIFIER ) {
    if ( !GetNextToken( head ) )  return false; // get token id
    else if ( !PeekNextToken( head, peekToken ) )  return false;
    else if ( strcmp( peekToken -> tokenStr, "(" ) == 0 ) {
      // '(' [ actual_parameter_list ] ')'
      if ( !CheckDefineFun( head ) ) return false; // call function
      else if ( !GetNextToken( head ) )  return false; // get token "("
      // 若gerror = false 代表沒有執行Actual_parameter_list
      else if ( !Actual_parameter_list( head ) && ( gerror || gNotFinish ) ) return false;
      else if ( !GetNextToken( head ) )  return false; // get token
      else if ( strcmp( head -> tokenStr, ")" ) == 0 ) return true;
      else {
        UnexpToken( head );
        return false;
      } // else
    } // else if
    else if ( strcmp( peekToken -> tokenStr, "[" ) == 0 ) {
      if ( !CheckDefineID( head ) ) return false; // 不是開頭直接確認
      if ( !GetNextToken( head ) ) return false; // get "["
      else if ( !Expression( head ) ) { // Expression 一定要做，false gerror have to be true
        if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
        return false;
      } // else if
      else if ( !GetNextToken( head ) )  return false;
      else if ( strcmp( head -> tokenStr, "]" ) != 0 ) {
        UnexpToken( head );
        return false;
      } // else if
      else if ( peekToken -> type == PP || peekToken -> type == MM ) {
        if ( !GetNextToken( head ) )  return false;
        return true;
      } // else if
      else return true;
    } // else if
    else {
      if ( !CheckDefineID( head ) ) return false;
      return true;
    } // else
  } // else if
  else if ( peekToken -> type == CONSTANT ) {
    if ( !GetNextToken( head ) )  return false; // get token Constant
    return true;
  } // else if
  else if ( CheckExpression( head ) ) return true; // 若不是括號unexpected token

  return false;

} // Unsigned_unary_exp()

bool CheckExpression( TokenPtr &head ) { // '(' expression ')'
  TokenPtr peekToken = NULL;
  if ( !gFirstTokenDone ) peekToken = head;
  else if ( !PeekNextToken( head, peekToken ) )  return false;

  if ( strcmp( peekToken -> tokenStr, "(" ) == 0 ) {
    if ( gFirstTokenDone ) {
      if ( !GetNextToken( head ) )  return false;
    } // if
    else gFirstTokenDone = true;

    if ( !Expression( head ) ) { // Expression 一定要做，false gerror have to be true
      if ( !gerror && !gNotFinish ) UnexpToken( head -> next );
      return false;
    } // if
    else if ( !GetNextToken( head ) )  return false; // get next token
    else if ( strcmp( head ->tokenStr, ")" ) != 0 ) {
      UnexpToken( head );
      return false;
    } // else if
    else return true;
  } // if
  else return false;
} // CheckExpression()

bool PeekNextToken( TokenPtr head, TokenPtr &peekToken ) {
  if ( head -> next != NULL ) {
    peekToken = head -> next;
    return true;
  } // if
  else {
    gNotFinish = true;
    return false;
  } // else
}  // PeekNextToken()

bool GetNextToken( TokenPtr &head ) {
  if ( head-> next != NULL ) head = head -> next;
  else { // head->next = NULL
    gNotFinish = true;
    return false;
  } // else

  return true;
} // GetNextToken()

void UnexpToken( TokenPtr head ) {
  char ch = '\0';
  gerror = true;
  printf( "> Line %d : unexpected token : \'%s\'\n",
          head -> firstAppearOn -> line, head ->tokenStr );
  if ( gNextChar == '\n' ) gLine--;
  if ( gLine == head -> firstAppearOn -> line && gNextChar != '\n' ) {
    scanf( "%c", &ch );
    while ( ch != '\n' )  scanf( "%c", &ch ); // read char till '\n'
  } // if
  else if ( gLine != head -> firstAppearOn -> line ) {
    TokenPtr temp = head -> next;
    if ( temp != NULL ) {
      gKeepToken = new char[30];
      strcpy( gKeepToken, temp -> tokenStr );
      gType = temp -> type;
      gLine--;
    } // if
  } // else if
} // UnexpToken()

void UndefinedToken( TokenPtr head ) {
  char ch = '\0';
  gerror = true;
  printf( "> Line %d : undefined identifier : \'%s\'\n",
          head -> firstAppearOn -> line, head -> tokenStr );
  if ( gNextChar == '\n'  ) gLine--; // 讀到 \n gLine會+1
  if ( gLine == head -> firstAppearOn -> line && gNextChar != '\n' ) {
    scanf( "%c", &ch );
    while ( ch != '\n' )  scanf( "%c", &ch ); // read char till '\n'
  } // if
  else if ( gLine != head -> firstAppearOn -> line ) {
    TokenPtr temp = head -> next;
    if ( temp != NULL ) {
      gKeepToken = new char[30];
      strcpy( gKeepToken, temp -> tokenStr );
      gType = temp -> type;
      gLine--;
      gNextCharLine--;
    } // if
  } // else if
} // UndefinedToken()

void GetTokenInfo( TokenPtr head ) {
  while ( head != NULL ) {
    ColumnPtr tempC = head -> firstAppearOn -> firstAppearAt;
    LinePtr tempL = head -> firstAppearOn ;
    printf( "%s", head -> tokenStr ) ;
    while ( tempL != NULL ) {
      while ( tempC != NULL ) {
        printf( " (%d,%d)", tempL -> line, tempC -> column );
        tempC = tempC -> next;
      } // while

      tempL = tempL -> next;
      if ( tempL != NULL )  tempC = tempL -> firstAppearAt;
    } // while

    head = head->next;
  } // while

  printf( "\n" );
} // GetTokenInfo()

void DeleteAllTokens( TokenPtr & head ) {
  while ( head != NULL ) {
    TokenPtr temp = head;
    head = head -> next; // 移動到下一個令牌
    delete[] temp -> tokenStr; // 釋放令牌字符串內存
    temp -> tokenStr = NULL;
    LinePtr lineTemp = temp -> firstAppearOn;
    while ( lineTemp != NULL ) {
      ColumnPtr columnTemp = lineTemp -> firstAppearAt;
      while ( columnTemp != NULL ) {
        ColumnPtr columnToDelete = columnTemp;
        columnTemp = columnTemp -> next;
        delete columnToDelete; // 釋放列內存
        columnToDelete = NULL;
      } // while

      LinePtr lineToDelete = lineTemp;
      lineTemp = lineTemp -> next;
      delete lineToDelete; // 釋放行內存
      lineToDelete = NULL;
    } // while

    delete temp; // 釋放令牌內存
    temp = NULL;
  } // while
} // DeleteAllTokens()
