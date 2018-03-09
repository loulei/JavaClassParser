package com.example.hello;

public class Hello implements HelloInterface, SecondInterface{

	private static String test = "test field string";
	public int i = 100;

	public byte[] data;

	static HelloInterface helloInter;

	public static void main(String[] argc){
		System.out.println("Hello, dex!");
		helloInter.sayHello();
	}

	public void sayHello(){
		System.out.println("hello interface");
	}

	public void secondHello(){
		System.out.println("second interface");
	}

}
