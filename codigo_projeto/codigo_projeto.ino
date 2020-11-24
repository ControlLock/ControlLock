#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
const byte filas = 4; 
const byte colunas = 3;
int angulo = 0;
Servo servo1;
char tecla[filas][colunas] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pinFilas[filas] = {7, 6, 5, 4}; 
byte pinColunas[colunas] = {1, 2, 3}; 
Keypad keypad = Keypad( makeKeymap(tecla), pinFilas, pinColunas, filas, colunas );
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long Senha=1234,ContraSenha=0,SenhaSistema;
int ContadorSenha=0,ContadorSenhaErrada=0,Ligar=0,NumeroA=0,NumeroB=0,NumeroC=0,NumeroD=0,VariavelX=0,Ativa=0,repete=0,explica=0,AtivaSenhaNova=0,opcao=0;
int  Speaker=A5,Porta=A1,SinalVisualVerde=A3, SinalVisualVermelho=A2;
String SenhaString,SenhaStringSistema;


void setup() {
  servo1.attach(A4);
  lcd.begin(16,2); 
  pinMode(Speaker, OUTPUT);
  pinMode(SinalVisualVerde, OUTPUT);
  pinMode(SinalVisualVermelho, OUTPUT);
  pinMode(Porta, INPUT_PULLUP);
  digitalWrite(SinalVisualVerde, HIGH);
  digitalWrite(SinalVisualVermelho, LOW);
}

void loop() {
  char NumeroDigitado = keypad.getKey(); 

  if (opcao==0&&Ligar==0){
    if(explica==0){
      digitalWrite(SinalVisualVerde, HIGH);
      digitalWrite(SinalVisualVermelho, LOW);
      lcd.setCursor(0,0);
      lcd.print("1-Liga Alarme");
      lcd.setCursor(0,1);
      lcd.print("2-Nova senha");
      delay(3000);
      lcd.clear();
      lcd.print("Opcao:");
      explica=1;
    }
    if (NumeroDigitado=='1'||NumeroDigitado=='2'){ 
      switch (NumeroDigitado){
        case '1':
          opcao=1;
          lcd.clear();
          explica=0;
          NumeroDigitado=' ';
          break;
        case '2':
          opcao=2;
          lcd.clear();
          explica=0;
          NumeroDigitado=' ';
          break;           
      }
    }
  }
  if (ContadorSenhaErrada<4){
    DisparaAlarme(NumeroDigitado);
    if(Ligar==0&&opcao==2){
      if(AtivaSenhaNova==0){
        ConfirmaSenha(NumeroDigitado);
      }
      if(AtivaSenhaNova==1){
        MudarSenha(NumeroDigitado);
        NumeroDigitado=' ';
      }
    }
    if(Ligar==0&&opcao==1){
      LigarAlarme(NumeroDigitado);
    }
    else if (Ligar==1&&Ativa!=1){
      DesligarAlarme(NumeroDigitado);
      if (ContadorSenhaErrada==3){
        lcd.clear();
        if(repete==0){
          digitalWrite(Speaker, HIGH);
          digitalWrite(SinalVisualVerde, HIGH);
          digitalWrite(SinalVisualVermelho, LOW);
          lcd.print("Alarme Disparado");
          delay(1000);
        } 
        lcd.clear();
        GeraCodigoSistema(1);
        GeraCodigoSistema(2);
        GeraCodigoSistema(3);
        lcd.setCursor(0,1);
        lcd.print("Pin:");
        NumeroDigitado=' ';
        Ativa=1;
      }
    }
    if (Ativa==1&&Ligar==1){           
      DesligarAlarmeSistema(NumeroDigitado); 
    }
  }  
}

void DisparaAlarme(char num){
  if(digitalRead(Porta)==LOW&&Ligar==1&&Ativa!=1){
    lcd.clear();
    digitalWrite(Speaker, HIGH);
    digitalWrite(SinalVisualVerde, LOW);
    digitalWrite(SinalVisualVermelho, HIGH);
    lcd.print("****INVASAO****");
    delay(1000);
    lcd.clear();
    repete=1;
    explica=0;
    DesligarAlarme(num);
  }
}

void DesligarAlarme(char num){
  if(explica==0){
    lcd.setCursor(0,0);
    lcd.print("Desligar Alarme");
    lcd.setCursor(0,1);
    lcd.print("Senha:");
    explica=1;
  }
  if (num=='0'||num=='1'||num=='2'||num=='3'||num=='4'||num=='5'||num=='6'||num=='7'||num=='8'||num=='9'&&ContadorSenha<4){
    lcd.print(num);
    SenhaString+=num;
    ContadorSenha++;
    if(ContadorSenha==4){
      ContraSenha=SenhaString.toInt();
      if(Senha==ContraSenha){
        SenhaCerta(1);
        digitalWrite(SinalVisualVerde, HIGH);
        digitalWrite(SinalVisualVermelho, LOW);
      }
      else{
       SenhaErrada(1);  
      } 
    }
  }  
}

void LigarAlarme(char num){
  if(explica==0){
    lcd.setCursor(0,0);
    lcd.print("Ligar Alarme");
    lcd.setCursor(0,1);
    lcd.print("#SuaSenha");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ligar Alarme");
    lcd.setCursor(0,1);
    lcd.print("Senha:");
    explica=1;
  }
  if (num=='#'||num=='0'||num=='1'||num=='2'||num=='3'||num=='4'||num=='5'||num=='6'||num=='7'||num=='8'||num=='9'&&ContadorSenha<5){
    lcd.print(num);
    ContadorSenha++;
    if(num!='#'){
      SenhaString+=num;
    }
    if(ContadorSenha==5){
      ContraSenha=SenhaString.toInt();
      if(Senha==ContraSenha){
        SenhaCerta(2);
        digitalWrite(SinalVisualVerde, LOW);
        digitalWrite(SinalVisualVermelho, HIGH);
      }
      else{
        SenhaErrada(2); 
      }
    }
  }
}

void ConfirmaSenha(char num){
  if(explica==0){
    lcd.setCursor(0,0);
    lcd.print("Mudar Senha");
    lcd.setCursor(0,1);
    lcd.print("*SuaSenha*");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mudar Senha");
    lcd.setCursor(0,1);
    lcd.print("Senha:");
    explica=1;
  }
  if (num=='*'||num=='0'||num=='1'||num=='2'||num=='3'||num=='4'||num=='5'||num=='6'||num=='7'||num=='8'||num=='9'&&ContadorSenha<6){ 
    lcd.print(num);
    ContadorSenha++;
    if(num!='*'){
      SenhaString+=num;
    }
    if(ContadorSenha==6){
      ContraSenha=SenhaString.toInt();
      if(Senha==ContraSenha){  
        AtivaSenhaNova=1;
        lcd.clear();
        SenhaString=" ";
        ContadorSenha=0;
      }
      else{
        SenhaErrada(3); 
      }
    }   
  }
}

void MudarSenha(char num){
  if(ContadorSenha==0){
    lcd.setCursor(0,0);
    lcd.print("Digite Sua Nova");
    lcd.setCursor(0,1);
    lcd.print("Senha:");
  }
  if (num=='0'||num=='1'||num=='2'||num=='3'||num=='4'||num=='5'||num=='6'||num=='7'||num=='8'||num=='9'&&ContadorSenha<4){
    ContadorSenha++;
    lcd.print(num);
    SenhaString+=num;
    if(ContadorSenha==4){  
      Senha=SenhaString.toInt();
      delay(1000);
      SenhaCerta(3);
    }
  }
}

void SenhaCerta(int x){
  lcd.clear(); 
  if(x == 1){
  lcd.print("Alarme Desligado");
  angulo  = 0; 
  servo1.write(map(angulo, 0, 1022, 0, 180)); 
  ContadorSenhaErrada=0;
  Ligar=0;
  digitalWrite(Speaker, LOW);
  digitalWrite(SinalVisualVerde, HIGH);
  digitalWrite(SinalVisualVermelho, LOW);
  }
  else if(x==2){
  lcd.print("Alarme Ligado");
  angulo  = 1022; 
  servo1.write(map(angulo, 0, 1022, 0, 180)); 
  Ligar=1;
  }
  else if(x==3){
  lcd.print("Senha Alterada");
  ContadorSenhaErrada=0;
  Ligar=0;
  }
  delay(2000);
  lcd.clear();
  opcao=0;
  repete=0;
  explica=0;
  SenhaString="";
  ContadorSenha=0;
}

void SenhaErrada(int x){
  lcd.clear(); 
  if(x==1){
    lcd.print("Senha Errada");
    ContadorSenhaErrada++;
  }
  else if(x==2){
    lcd.print("Senha Errada");
    lcd.setCursor(0,1);
    lcd.print("Nao foi ligado");
    Ligar=0;
  } 
  else if(x==3){
    lcd.print("Senha Errada");
    opcao=0;
  } 
  delay(2000);
  lcd.clear();
  explica=0;
  SenhaString="";
  ContadorSenha=0;
}

void DesligarAlarmeSistema(char num){
  if (num=='0'||num=='1'||num=='2'||num=='3'||num=='4'||num=='5'||num=='6'||num=='7'||num=='8'||num=='9'&&ContadorSenha<7){
    lcd.print(num);
    SenhaString+=num;
    ContadorSenha++;
    if(ContadorSenha==7){
      ContraSenha=SenhaString.toInt();
      if(SenhaSistema==ContraSenha){
        lcd.clear();
        lcd.print("Alarme Desligado");
        angulo  = 0; 
        servo1.write(map(angulo, 0, 1022, 0, 180)); 
        delay(1000);
        lcd.clear(); 
        Ativa=0;
        Ligar=0;
        opcao=0;
        repete=0;
        SenhaString="";
        ContadorSenha=0;
        SenhaStringSistema=""; 
        ContadorSenhaErrada=0;
        digitalWrite(Speaker, LOW);
        digitalWrite(SinalVisualVerde, HIGH);
        digitalWrite(SinalVisualVermelho, LOW);
      }
      else{
        lcd.clear();
        Ligar=1;
        Ativa=0;
        repete=1;
        SenhaString="";
        ContadorSenha=0;
        SenhaStringSistema="";
      }
    }
  }
}

void GeraCodigoSistema(int x){
  if(x==1){
    NumeroA=rand()% 9;
    NumeroB=rand()% 9;
    NumeroC=rand()% 4;
    NumeroD=rand()% 4; 
  }
  else if(x==2){
    lcd.setCursor(0,0);
    lcd.print("Codigo");
    lcd.setCursor(7,0);
    lcd.print(NumeroA);
    lcd.setCursor(8,0);
    lcd.print(NumeroB);
    lcd.setCursor(9,0);
    lcd.print(NumeroC);
    lcd.setCursor(10,0);
    lcd.print(NumeroD);
  }
  else if(x==3){
    VariavelX=NumeroC+NumeroD;
    SenhaStringSistema+=VariavelX;
    SenhaStringSistema+=NumeroA;
    SenhaStringSistema+=NumeroD;
    SenhaStringSistema+=VariavelX;
    SenhaStringSistema+=NumeroC;
    SenhaStringSistema+=NumeroB;
    SenhaStringSistema+=VariavelX;
    SenhaSistema=SenhaStringSistema.toInt();
  }
}