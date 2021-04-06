#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        13 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



char ssid[] = "@epslinares_uja";                     
char key[]="";       
int estado = WL_IDLE_STATUS;

IPAddress ip(192,168,43,100);     
IPAddress gateway(192,168,43,1);   
IPAddress subnet(255,255,255,0);   



int modo=1;

int rr=254;
int gg=254;
int bb=254;

int R=0;
int G=0;
int B=0;

int Rini=0;
int Gini=0;
int Bini=0;

float incrementoR=0;
float incrementoG=0;
float incrementoB=0;
    
    
int cont=0;
int contcolor=0;

String val="";

WiFiServer server(80);

void setup() {
  
  Serial.begin(115200);

  Serial.println("Lampara Maria !!!!!");
    
  pinMode(LED_BUILTIN, OUTPUT);
 
  while(estado!=WL_CONNECTED) {    
    digitalWrite(LED_BUILTIN, HIGH);  
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);  
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);      
    
    WiFi.mode(WIFI_STA);
    WiFi.config(ip, gateway, subnet);
    estado=WiFi.begin(ssid,key);
    delay(100);
    estado=WiFi.status();
  }
  digitalWrite(LED_BUILTIN, LOW); //Este led se ilumina al contrario
  

 
  server.begin();

  
  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}



void loop() {

  if(modo==0) {   
   
    cont++;
    if(cont>=16) {
      pixels.clear(); // Set all pixel colors to 'off'
      R=random(0,254);G=random(0,254);B=random(0,254);
      cont=0;    
    }
    pixels.setPixelColor(cont, pixels.Color(R,G,B));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(50); // Pause before next pass through loop
    
    
  }else if(modo==1) {   
    
    cont++;
    if(cont>=200) {
      pixels.clear(); // Set all pixel colors to 'off'
      Rini=random(0,254);Gini=random(0,254);Bini=random(0,254);
      incrementoR=(int)(Rini/200);
      incrementoG=(int)(Gini/200);
      incrementoB=(int)(Bini/200);    
      cont=0;    
    }

    R=(int)(R+incrementoR);G=(int)(G+incrementoG);B=(int)(B+incrementoB);
    
    for(int i=0;i<16;i++) {  pixels.setPixelColor(i,pixels.Color(R,G,B)); }

    pixels.show();
    delay(50);
    
  }else if(modo==2) {   

    for(int i=0; i<NUMPIXELS; i++) { pixels.setPixelColor(i,pixels.Color(rr,gg,bb)); }
    pixels.show();
    delay(50);
      
  }
  
  
  WiFiClient client = server.available();
  if (!client) { return; }      // sino hay cliente volvemos al loop

  String req="";
 
  int contador=0;
  while(!client.connected()) {delay(1);contador++;if(contador>1000){return;}}
  contador=0;
  while(!client.available()) {delay(1);contador++;if(contador>1000){return;}}

  char c;
  do{          
    c = client.read();
    req+=c; 
    //Serial.println(req);Serial.println("\r\n");       
  }while(c!='\n');

  String s = "";
  
  if (req.indexOf("/movimiento")!=-1){   
    modo=0;                
    s="OK";
  }else if(req.indexOf("/difuminado")!=-1){  
    modo=1;
    s="OK";    
  }else if (req.indexOf("/color")!=-1){  
    int pos1=req.indexOf("/color");
    int pos2=req.indexOf(";",pos1);

    val=req.substring(pos1+6,pos2);
    rr=val.toInt();
  //  Serial.print("|");Serial.print(val);
    
    pos1=req.indexOf(";",pos2+1);
    val=req.substring(pos2+1,pos1);
    gg=val.toInt();
 //   Serial.print(",");Serial.print(val);
    
    pos2=req.indexOf(";",pos1+1);
    val=req.substring(pos1+1,pos2);
    bb=val.toInt();
  //  Serial.print(",");Serial.print(val);Serial.print("|");
    
    
    
    
    
    modo=2;    
  }else {
    s=F("<html>\n\n<head><meta http-equiv='Content-Type' content='text/html;charset=UTF-8'><title id='titulo'>LAMPARA LEDS MARIA</title>\n<script type='text/javascript'>\n\nvar isMobile={\nmobilecheck : function(){\nreturn(/(android|bb\\d+|meego).+mobile|avantgo|bada\\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\\.(browser|link)|vodafone|wap|wis (ce|phone)|xda|xkno|android|ipad|playbook|silk/i.test(navigator.userAgent||navigator.vendor||wi.opera)||/1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\\-(n|u)|c55\\/|capi|ccwa|cdm\\-|cell|chtm|cldc|cmd\\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\\-s|devi|dica|dmob|do(c|p)o|ds(12|\\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\\-|_)|g1 u|g560|gene|gf\\-5|g\\-mo|go(\\.w|od)|gr(ad|un)|haie|hcit|hd\\-(m|p|t)|hei\\-|hi(pt|ta)|hp( i|ip)|hs\\-c|ht(c(\\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\\-(20|go|ma)|i230|iac( |\\-|\\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\\/)|klon|kpt |kwc\\-|kyo(c|k)|le(no|xi)|lg( g|\\/(k|l|u)|50|54|\\-[a-w])|libw|lynx|m1\\-w|m3ga|m50\\/|ma(te|ui|xo)|mc(01|21|ca)|m\\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\\-2|po(ck|rt|se)|prox|psio|pt\\-g|qa\\-a|qc(07|12|21|32|60|\\-[2-7]|i\\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\\-|oo|p\\-)|sdk\\/|se(c(\\-|0|1)|47|mc|nd|ri)|sgh\\-|shar|sie(\\-|m)|sk\\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\\-|v\\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\\-|tdg\\-|tel(i|m)|tim\\-|t\\-mo|to(pl|sh)|ts(70|");client.print(s);
    s=F("m\\-|m3|m5)|tx\\-9|up(\\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\\-|your|zeto|zte\\-/i.test((navigator.userAgent||navigator.vendor||wi.opera).substr(0,4)))\n}\n}\n\n\nvar movil=false;\nvar firefox=false;\nvar wtx=0;\nvar wty=0;\n\nvar xc=0;\nvar xy=0;\n\nvar xc2=0;\nvar xy2=0;\n\nvar px=0;\nvar py=0;\n\nvar pwtx=0;\n\nvar wi=window;\n\nvar rx;\nvar gx;\nvar bx;\n\nvar rr=128;\nvar gg=128;\nvar bb=128;\n\n\nvar sz;\n\nvar difuminado=false;\nvar movimiento=false;\n\nvar moviendo=false;\n\n\nfunction dg(z){return document.getElementById(z);}\n\nfunction init(fps){\n    \nvar navegador=navigator.userAgent;\nvar nav=navegador.toLowerCase(); \n\nif(nav.indexOf('firefox')!=-1){firefox=true;}\n\nmovil=isMobile.mobilecheck();\n\nc=dg('c');\n\nif(!firefox){\nc.addEventListener('touchstart',rp,false);\nc.addEventListener('touchmove',rm,false);\nc.addEventListener('touchend',rpu,false);\n}\n\nc.addEventListener('mousedown',rp,false);\nc.addEventListener('mousemove',rm,false);\nc.addEventListener('mouseup',rpu,false);\n\n\na();\n\n\nif(c.getContext){\ng=c.getContext('2d');g.canvas.width=xc;g.canvas.height=yc;\nsetInterval(upd,1000);\n    }\n\nrx=xc/2;\ngx=xc/2;\nbx=xc/2;\n\n\n\n}\n\nfunction get(peticion){\n\npx=0;py=0;\nip=location.host;\n\nif(wi.XMLHttpRequest){xmlhttp=new XMLHttpRequest();}\nelse{xmlhttp=new ActiveXObject('Microsoft.XMLHTTP');}\n\nxmlhttp.onreadystatechange=function(){\nif(xmlhttp.responseText!='' && xmlhttp.readyState==4 && xmlhttp.status==200){\ndatorecogido=xmlhttp.responseText;\n}else if(xmlhttp.status==404){alert('FALLO COMUNICACION !!');}\n}\n\n//alert(peticion);\n\nxmlhttp.open('GET','http://'+ip+'/'+peticion,true);\nxmlhttp.send();\n\n\n}\n\nfullscreen = function(e){\n      if (e.webkitRequestFullScreen) {e.webkitRequestFullScreen();}\n  else if(e.mozRequestFullScreen) {e.mozRequestFullScreen();}\n}\n\nfunction mm(e){\nif((movil)&&(!firefox)){\npx=e.touches[0].pageX-pwtx;\npy=e.touches");client.print(s);
    s=F("[0].pageY-7;\n}else{\npx=e.clientX-pwtx;\npy=e.clientY+wi.scrollY-7;\n}\n}\n\nfunction rpu(e){ px=0;py=0;rpulsado=false;upd();}\n\nfunction rp(e){ mm(e);rpulsado=true;upd();}\n\nfunction rm(e){\nif(rpulsado) {\nmoviendo=true;\nmm(e);upd();\nmoviendo=false;\n}\n}\n\nfunction a(){\n\nif((wtx!=wi.innerWidth)||(wty!=wi.innerHeight)){\n\nwtx=wi.innerWidth;\nwty=wi.innerHeight;\n\nxc=wtx*0.98;\nyc=wty*0.97;\n\npwtx=((wtx-xc)/2);\n\nc.style.left=pwtx;\nc.width=xc;c.height=yc;\n\n}\n\n\n}\n\nfunction gb(){g.beginPath();}\nfunction gf(){g.fill();}\nfunction gc(){g.closePath();}\nfunction gs(){g.stroke();}\nfunction gm(x,y){g.moveTo(x,y);}\nfunction gl(x,y){g.lineTo(x,y);}\n\nfunction r(v,x,y,tx,ty){gb();g.rect(x,y,tx,ty);g.fillStyle=v;gf();g.lineWidth=2;g.strokeStyle='black';gs();gc();}\n\n\nfunction upd(){\n\nmovimiento=false;\ndifuminado=false;\n\na();\nr('#DDDDDD',0,0,xc,yc);\nr('#000000',px,py,2,2);\n\nif((px>(xc/4))&&(px<((xc/4)+(xc/2)))){\n\nif((py>((yc*3)/20))&&(py<(((yc*3)/20)+(yc/10)))){\nrx=px;\nvar valor=(((xc/4)+(xc/2))-px)/(xc/2);\nvalor=valor*255;\nrr=255-Number.parseInt(valor,10);\n\nif(!moviendo) { get('color'+rr+';'+gg+';'+bb+';'); }\n}\n\nif((py>((yc*6)/20))&&(py<(((yc*6)/20)+(yc/10)))){\ngx=px;\nvar valor=(((xc/4)+(xc/2))-px)/(xc/2);\nvalor=valor*255;\ngg=255-Number.parseInt(valor,10);\n\nif(!moviendo) { get('color'+rr+';'+gg+';'+bb+';'); }\n\n}\n\nif((py>((yc*9)/20))&&(py<(((yc*9)/20)+(yc/10)))){\nbx=px;\nvar valor=(((xc/4)+(xc/2))-px)/(xc/2);\nvalor=valor*255;\nbb=255-Number.parseInt(valor,10);\n\nif(!moviendo) { get('color'+rr+';'+gg+';'+bb+';'); }\n}\n\n}\n\n\nr('#000000',xc/4,(yc*3)/20,xc/2,yc/10);\nr('#FFFFFF',xc/4+2,((yc*3)/20)+2,(xc/2)-4,(yc/10)-4);\nr('#FF0000',rx-20,(yc*3)/20,40,yc/10);\n\nr('#000000',xc/4,(yc*6)/20,xc/2,yc/10);\nr('#FFFFFF',xc/4+2,((yc*6)/20)+2,(xc/2)-4,(yc/10)-4);\nr('#00FF00',gx-20,((yc*6)/20)+2,40,(yc/10)-4);\n\nr('#000000',xc/4,(yc*9)/20,xc/2,yc/10);\nr('#FFFFFF',(xc/4)+2,((yc*9)/20)+2,(xc/2)");client.print(s);
    s=F("-4,(yc/10)-4);\nr('#0000FF',bx-20,((yc*9)/20)+2,40,(yc/10)-4);\n\nsz=xc/30;\n\n\nif((px>(xc/4))&&(px<((xc/4)+(xc/5)))){\nif((py>((yc*7)/10))&&(py<(((yc*7)/10)+(yc/10)))){\ndifuminado=true;\nif(!moviendo) {  get('difuminado'); }\n}\n}\n\nr('#000000',xc/4,(yc*7)/10,xc/5,yc/10);\n\nif(difuminado) {\nr('#CCCCFF',(xc/4)+2,((yc*7)/10)+2,(xc/5)-4,(yc/10)-4);\n}else{\nr('#8888FF',(xc/4)+2,((yc*7)/10)+2,(xc/5)-4,(yc/10)-4);\n}\n\ng.fillStyle='black';\ng.font='bold '+sz+'px Arial';\ng.fillText('Difuminado',(xc*26)/100,((yc*77)/100)+2);\n\n\nif((px>((xc*11)/20))&&(px<(((xc*11)/20)+(xc/5)))){\nif((py>((yc*7)/10))&&(py<(((yc*7)/10)+(yc/10)))){\nmovimiento=true;\nif(!moviendo) { get('movimiento'); }\n}\n}\n\n\nr('#000000',(xc*11)/20,(yc*7)/10,xc/5,yc/10);\n\nif(movimiento) {\nr('#CCCCFF',((xc*11)/20)+2,((yc*7)/10)+2,(xc/5)-4,(yc/10)-4);\n}else{\nr('#8888FF',((xc*11)/20)+2,((yc*7)/10)+2,(xc/5)-4,(yc/10)-4);\n}\n\n\ng.fillStyle='black';\ng.font='bold '+sz+'px Arial';\ng.fillText('Movimiento',(xc*56)/100,((yc*77)/100)+2);\n\n\n\n}\n\n</script></head><body onload='init(30)' id='bodi' style='overflow-y:hidden;overflow-x:hidden'>\n<center><canvas id='c' tabindex='0' style='border:1px solid #000000;'></canvas></center></body></html>");client.print(s);
    s=F("");
 
  }

  client.print(s);
  client.print("\r\n\r\n");   


  client.flush();
  client.stop();   
  client.stopAll();  
  
}
