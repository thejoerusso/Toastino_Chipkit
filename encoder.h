
// define routing characters
char* extra_routings[] = { "LG MIC", "LG LINE", "LG BUS", "LG TAPE", "LG STA", "LG STB", "LG STC", "LG PAN", "LG MTX", "SM MIC", "SM LINE", "SM BUS", "SM TAPE", "SM STA", "SM STB", "SM STC", "SM PAN", "SM MTX" }; 

// define indices to lookup routings against the extra_routings char array
// TODO : find a way to clean this up without burdening memory with HashMap library or the like..
const int ROTARY_LG_MIC  = 1;
const int ROTARY_LG_LINE = 2;
const int ROTARY_LG_BUS  = 3;
const int ROTARY_LG_TAPE = 4;
const int ROTARY_LG_STA  = 5;
const int ROTARY_LG_STB  = 6;
const int ROTARY_LG_STC  = 7;
const int ROTARY_LG_PAN  = 8;
const int ROTARY_LG_MTX  = 9;

const int ROTARY_SM_MIC  = 10;
const int ROTARY_SM_LINE = 11;
const int ROTARY_SM_BUS  = 12;
const int ROTARY_SM_TAPE = 13;
const int ROTARY_SM_STA  = 14;
const int ROTARY_SM_STB  = 15;
const int ROTARY_SM_STC  = 16;
const int ROTARY_SM_PAN  = 17;
const int ROTARY_SM_MTX  = 18;

int routings    = 0;
int maxroutings = 48;

boolean encoderToggle = LOW;


