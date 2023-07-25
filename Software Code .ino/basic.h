#define FILESYSTEM SPIFFS

#define BUTTON_A 15
#define BUTTON_B 32
#define BUTTON_C 14

#define FORMAT_SPIFFS_IF_FAILED true

#define SENSOR_TIME_GAP 3000
#define RESET_REQ_TIME 500

//New Board
//#define BATPIN A13 //BATT_MONITOR
//#define SERIAL_S Serial1

//Old Board
#define BATPIN A13
#define SERIAL_S Serial2

#define ABOVE_GROUND 0
#define BELOW_GROUND 1

#define DELETE_NO_RESTART 0
#define DELETE_ONLY_DATA  1
#define DELETE_ALL_FILES  2


File file;

int sysmode = 0, run_no = 0;



const char ssid[30]     = "H2OBukt99"; //SSID
const char password[30] = "123456789"; //Password

WebServer server(80);
//holds the current upload
File fsUploadFile;

char appenNewLine[5] = "\r\n";
char spaceLine[3] = " ";
char plusLine[200];

bool restartOnHttpDELETE = false;
//BluetoothSerial SerialBT;

char HTMLText[7000];
char allsettings[700];
char all_no[10];

//Inital Inputs
long  inputsTimer       = 0;
int   inputMode         = 0; 
int   Ring_Diameter     = 160;
int   Ring_Insertion    = 30;
int   Soil_Type         = 2;// MH changed
int   Depth_of_Water    = 10; // above ground
int   Hole_diameter     = 50; // below ground
int   Depth_of_WaterB   = 100; // below ground
int   Hole_Depth        = 100; // below ground

float Alpha[4] = {0.001f, 0.004f, 0.012f, 0.036f};

int   settIndex = 0, rIndex = 0;
int   oldDepthIndex = 0;

int size_of_file = 0, deleteFileLevel = -1;
float IR, HC, avgDepth = 0, avgDepth_old = 0;
float old_depths[1000];

unsigned long IR_Time_GAP = 60000UL;//mh rtn to 60 sec
