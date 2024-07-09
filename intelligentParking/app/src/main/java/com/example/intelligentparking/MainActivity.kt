package com.example.intelligentparking

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.example.intelligentparking.databinding.ActivityMainBinding
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.FirebaseDatabase

class MainActivity : AppCompatActivity() {
    private lateinit var binding : ActivityMainBinding
    private lateinit var database: DatabaseReference
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val number = 4
        binding.lefteditTextNumber.setText(number.toString())
        binding.rightButton.setOnClickListener { capacity() }


    }

    private fun capacity() {
        database = FirebaseDatabase.getInstance().getReference("ParkingInfo")
        database.child("Availability").get().addOnSuccessListener {
            if(it.exists()) {
                val capacity:Int = it.value.toString().toInt()
                Toast.makeText(this, "Updated successfully!", Toast.LENGTH_SHORT).show()
                binding.editTextNumber.setText(capacity.toString())
//
            }
            else{
                Toast.makeText(this, "ParkingInfo/Availability does not exist", Toast.LENGTH_LONG).show()
            }
        }.addOnFailureListener(){
            Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show()
        }

        database.child("Slot_1").get().addOnSuccessListener {
            if(it.exists()) {
                val slot_1:String = it.value.toString()
                //Toast.makeText(this, "Updated successfully!", Toast.LENGTH_SHORT).show()
                binding.slot1.setText(slot_1)
//
            }
            else{
                Toast.makeText(this, "ParkingInfo/Slot_1 does not exist", Toast.LENGTH_LONG).show()
            }
        }.addOnFailureListener(){
            Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show()
        }

        database.child("Slot_2").get().addOnSuccessListener {
            if(it.exists()) {
                val slot_2:String = it.value.toString()
                //Toast.makeText(this, "Updated successfully!", Toast.LENGTH_SHORT).show()
                binding.slot2.setText(slot_2)
//
            }
            else{
                Toast.makeText(this, "ParkingInfo/Slot_2 does not exist", Toast.LENGTH_LONG).show()
            }
        }.addOnFailureListener(){
            Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show()
        }

        database.child("Slot_3").get().addOnSuccessListener {
            if(it.exists()) {
                val slot_3:String = it.value.toString()
                //Toast.makeText(this, "Updated successfully!", Toast.LENGTH_SHORT).show()
                binding.slot3.setText(slot_3)
//
            }
            else{
                Toast.makeText(this, "ParkingInfo/Slot_3 does not exist", Toast.LENGTH_LONG).show()
            }
        }.addOnFailureListener(){
            Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show()
        }

        database.child("Slot_4").get().addOnSuccessListener {
            if(it.exists()) {
                val slot_4:String = it.value.toString()
                //Toast.makeText(this, "Updated successfully!", Toast.LENGTH_SHORT).show()
                binding.slot4.setText(slot_4)
//
            }
            else{
                Toast.makeText(this, "ParkingInfo/Slot_4 does not exist", Toast.LENGTH_LONG).show()
            }
        }.addOnFailureListener(){
            Toast.makeText(this, "FAILED", Toast.LENGTH_SHORT).show()
        }

    }

}
