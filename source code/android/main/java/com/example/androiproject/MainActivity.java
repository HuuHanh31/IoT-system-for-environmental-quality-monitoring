package com.example.androiproject;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.nio.charset.Charset;

public class MainActivity extends AppCompatActivity {
    MQTTHelper mqttHelper;
    TextView txtCOD;
    TextView txtSS;
    TextView txtNH4;
    TextView txtNO3;
    TextView txtpH;

    ToggleButton togglerl1;
    ToggleButton togglerl2;
    ToggleButton togglerlm;
//    ToggleButton btnmanual;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        txtCOD = findViewById(R.id.cod);
        txtSS = findViewById(R.id.ss);
        txtNH4 = findViewById(R.id.nh4);
        txtNO3 = findViewById(R.id.no3);
        txtpH = findViewById(R.id.ph);
         togglerl1 = findViewById(R.id.rl1);
        togglerl2 = findViewById(R.id.rl2);
        togglerlm = findViewById(R.id.rlm);
        //btnmanual = findViewById(R.id.manual);
        //txtTemp.setText("90" + "°C");


        startMQTT();
        togglerl1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b == true){
                    Log.d("mqtt", "Button is ON ");
                    sendDataToMQTT("chanh/feeds/TKLL_RL1","1");
                }
                else{
                    Log.d("mqtt","Button is OFF ");
                    sendDataToMQTT("chanh/feeds/TKLL_RL1","0");
                }
            }
        });
        togglerl2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b == true){
                    Log.d("mqtt", "Button is ON ");
                    sendDataToMQTT("chanh/feeds/TKLL_RL2","1");
                }
                else{
                    Log.d("mqtt","Button is OFF ");
                    sendDataToMQTT("chanh/feeds/TKLL_RL2","0");
                }
            }
        });
        togglerlm.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(b == true){
                    Log.d("mqtt", "Button is ON ");
                    sendDataToMQTT("chanh/feeds/TKLL_RLM","1");
                }
                else{
                    Log.d("mqtt","Button is OFF ");
                    sendDataToMQTT("chanh/feeds/TKLL_RLM","0");
                }
            }
        });
//        btnmanual.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
//            @Override
//            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
//                if(b == true){
//                    Log.d("mqtt", "Button is ON ");
//                    sendDataToMQTT("Chanhcei/feeds/IoT_manual","ON");
//                }
//                else{
//                    Log.d("mqtt","Button is OFF ");
//                    sendDataToMQTT("Chanhcei/feeds/IoT_manual","OFF");
//                }
//            }
//        });
    }
    private void sendDataToMQTT(String topic,String mess){
        MqttMessage msg = new MqttMessage();
        msg.setId(1234);
        msg.setQos(0);
        msg.setRetained(true);

        byte[] b = mess.getBytes(Charset.forName("UTF-8"));
        msg.setPayload(b);
        try{
            mqttHelper.mqttAndroidClient.publish(topic,msg);
        }
        catch (Exception e){}
    }
    private void startMQTT(){
        mqttHelper = new MQTTHelper(getApplicationContext(),"456");
        mqttHelper.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                Log.d("Mqtt","ket noi thanh cong");
            }
            @Override
            public void connectionLost(Throwable cause) {
                Log.d("Mqtt","ket noi khong thanh cong");
            }
            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
                Log.d("mqtt","Received: " + message.toString());
                if(topic.contains("TKLL_COD")){
                    txtCOD.setText("COD: " + message.toString() + "%");
                }
                if(topic.contains("TKLL_SS")){
                    txtSS.setText("SS: " + message.toString() + "%");
                }
                if(topic.contains("TKLL_NH4")){
                    txtNH4.setText("NH4: " + message.toString() + "%");
                }
                if(topic.contains("TKLL_NO3")){
                    txtNO3.setText("NO3: " + message.toString() + "%");
                }
                if(topic.contains("TKLL_pH")){
                    txtpH.setText("pH: " + message.toString() + "%");
                }

                if(topic.contains("TKLL_RL1")){
                    if (message.toString().equals("0")){
                        togglerl1.setChecked(false);
                    }
                    else{
                        togglerl1.setChecked(true);
                    }
                }
                if(topic.contains("TKLL_RL2")){
                    if (message.toString().equals("0")){
                        togglerl2.setChecked(false);
                    }
                    else{
                        togglerl2.setChecked(true);
                    }
                }
                if(topic.contains("TKLL_RLM")){
                    if (message.toString().equals("0")){
                        togglerlm.setChecked(false);
                    }
                    else{
                        togglerlm.setChecked(true);
                    }
                }
            }
            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });
    }
}