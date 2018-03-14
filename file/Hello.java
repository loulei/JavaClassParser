package com.example.hello;

public class Hello implements HelloInterface, SecondInterface{

	private static final String test = "test field string";
	public int i = 100;

	public byte[] data;

	static HelloInterface helloInter;

	public static void main(String[] argc){
		System.out.println("Hello, dex!");
		try {
			helloInter.sayHello();
		} catch (NullPointerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}

	public void sayHello(){
		System.out.println("hello interface");
	}

	public void secondHello(){
		System.out.println("second interface");
	}

}
