const int vars = 3;
bool sensorStatus[vars];
int statValue[] = {1,2,4};
int heatpump[] = {2,3,4}; //3
int gasHeat[] = {1,2,3,5}; //4
int laskuri = 0;
/*while(laskuri<3)*/while(true){
    char buffer[64] = "";
    MQTT::Message msg;
    msg.qos = MQTT::QOS0;
    msg.retained = false;
    msg.dup = false;
for(int b=0; b<vars;b++){sensorStatus[b]=false;}
float inTemp = inSensor.read();
float outTemp = outSensor.read();
/*float inTemp = sisalampo[laskuri]; Vaihtaa lämpötilaa
float outTemp = ulkolampo[laskuri]; simulaatiossa */
 inTemp=(inTemp*3.3f-0.5f)/0.01f;
 outTemp=(outTemp*3.3f-0.5f)/0.01f-10.0f;
printf("Inside temperature is %.1f\n\nOutside temperature is %.1f\n\n", inTemp, outTemp);

if(outTemp<15.0f)
   {
     if(outTemp<(-15.0f)){sensorStatus[0]=true;}
     if(inTemp<7.0f){sensorStatus[1]=true;}
     if(inTemp>15.0f){sensorStatus[2]=true;}
   
int status = 0;
//printf("%d", status); //debug
   for(int i=0; i<vars; i++){
    if(sensorStatus[i]==true){status = status+statValue[i];}
}
//printf("Status %d", status); //debug
bool pumpon = false;    //Muuttuja joka kertoo laitetaanko laite päälle.
bool gason = false;     //Poistaa toistot payloadista
   for(int j=0 ; j<3 ; j++){
        if(heatpump[j]==status){
         pumpon = true;}}
        if(pumpon==true){ 
       //printf("Testi %d Heatpump on\n", laskuri);
       sprintf(buffer + strlen(buffer), "Heatpump on\n");
       }
        else{
        //printf("Testi %d Heatpump off", laskuri);
       sprintf(buffer + strlen(buffer), "Heatpump off\n");
       }
   

    for(int h=0; h<4; h++){
       if(gasHeat[h]==status){
           gason=true;}
    } 
        if(gason==true){
       //printf("Testi %d Gasheat on\n", laskuri);
       sprintf(buffer + strlen(buffer), "Gasheat on\n");
       }
        else{
           //printf("Testi %d Gasheat off\n", laskuri);
           sprintf(buffer + strlen(buffer), "Gasheat off");
       }
     
    }
else{sprintf(buffer + strlen(buffer),"Heatpump off\n\nGasheat off\n");
//printf("Testi %d Heatpump off\n\nGasheat off\n\n", laskuri);
}
printf("\nBuffer %d: %s\n", laskuri, buffer);
//sprintf(buffer + strlen(buffer), "Laskuri %d", laskuri);
msg.payload = (void*)buffer;
msg.payloadlen = strlen(buffer);
client.publish(MBED_CONF_APP_MQTT_TOPIC, msg);   
   
 ThisThread::sleep_for(100s);
 laskuri++;
  } 
}
