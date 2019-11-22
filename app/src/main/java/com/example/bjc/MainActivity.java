package com.example.bjc;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button controllerButton;
    private Button instructionButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        controllerButton = findViewById(R.id.button2);
        instructionButton = findViewById((R.id.button));

        controllerButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startController();
            }
        });
        instructionButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startInstruction();
            }
        });
    }
    public void startController()
    {
        Intent intent = new Intent(this, Controller.class);
        startActivity(intent);
    }
    public void startInstruction()
    {
        Intent intent = new Intent(this, InstructionMenu.class);
        startActivity(intent);
    }
}
