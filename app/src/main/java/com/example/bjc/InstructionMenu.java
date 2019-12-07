package com.example.bjc;

import android.content.Intent;
import android.os.Bundle;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

public class InstructionMenu extends AppCompatActivity {
    private ImageButton assemblyButton;
    private ImageButton wiringButton;
    private ImageButton programmingButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_instruction);
        Toolbar toolbar = findViewById(R.id.toolbar);
        toolbar.setTitle("Instruction Menu");
        setSupportActionBar(toolbar);
        assemblyButton = findViewById(R.id.btnAssembly);
        wiringButton = findViewById(R.id.btnWiring);
        programmingButton = findViewById(R.id.btnProgramming);
        assemblyButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startAssembly();
            }
        });
        wiringButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startWiring();
            }
        });
        programmingButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startProgramming();
            }
        });
    }
    private void startAssembly() {
        Intent intent = new Intent(this, welcome_code.class);
        startActivity(intent);
    }
    private void startWiring() {
        Intent intent = new Intent(this, Controller.class);
        startActivity(intent);
    }
    private void startProgramming() {
        Intent intent = new Intent(this, Controller.class);
        startActivity(intent);
    }

}
