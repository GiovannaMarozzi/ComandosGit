#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int getFingerprintIDez();

//Conexao com o wifi
WiFiClient client;
char ssid[] = "CasaMarozzi";    // your SSID
char password[] = "AEGISMCM";       // your SSID Password
byte mac[6];

char digital[] = "";

#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
MySQL_Connection conn((Client *)&client);

char select[] = "select nome from biometria.users where nome ='%s'";
char query[80];

//Conexao com o BD
IPAddress server_addr(85, 10, 205, 173); // IP of the MySQL *server* here
char db_user[] = "giovanna";              // MySQL user login username
char db_password[] = "46386952168";        // MySQL user login password

MySQL_Cursor cur = MySQL_Cursor(&conn);

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  WiFi.begin(ssid, password);
  Serial.println("Conectando no Banco de dados...");
  delay(500);
  Serial.println("Conectando com o Leitor Biométrico...");
  if (conn.connect(server_addr, 3306, db_user, db_password)) {
  }
  else
    Serial.println("Connection failed.");
}

void loop() {
   
  row_values *row = NULL;
    long head_count = 0;

    //delay(1000);

    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    sprintf(query, select , "Giovanna"); //busca os dados no banco... Terceiro parâmentro é a variavel do passageiro
    cur_mem->execute(query); //executa a query informada
    column_names *columns = cur_mem->get_columns();

    String NomePassageiro;

    // Read the row (we are only expecting the one)
    do {
    row = cur_mem->get_next_row();
    if (row != NULL) {
      NomePassageiro = (row->values[0]);
    }
    } while (row != NULL);
    // Deleting the cursor also frees up memory used
    delete cur_mem;

    // Execute the select
    cur.execute(select);
    // Fetch the columns (required) but we don't use them.
    cur.get_columns();
    // Read the row (we are only expecting the one)
    do {
    row = cur.get_next_row();
    if (row != NULL) {
      head_count = atol(row->values[0]);
    }
    } while (row != NULL);
    // Now we close the cursor to free any memory
    cur.close();

    // Resultado da pesquisa

    Serial.print(" O resultado da sua pesquisa é: ");
    Serial.println(NomePassageiro);
//A ideia agora é colocar apenas uma parte do código para a captura da digital para que a mesma possa ir em uma variável e assim entrar no BD, abrir exemplos do enroll
//  getFingerprintID();
}

/*uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem Capturada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Dedo nao Localizado");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro ao se comunicar");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro ao Capturar");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }
  
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Digital Encontrada");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro ao se comunicar");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Digital Desconhecida");
    return p;
  } else {
    Serial.println("Erro Desconhecido");
    return p;
  }   
  
  Serial.print("ID # Encontrado"); 
  Serial.print(finger.fingerID); 
  Serial.print(" com precisao de "); 
  Serial.println(finger.confidence);
  return finger.fingerID;  
}*/
