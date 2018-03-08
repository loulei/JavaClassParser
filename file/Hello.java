public class Hello{

	private static String test = "test field string";
	public int i = 100;

	static HelloInterface helloInter;

	public static void main(String[] argc){
		System.out.println("Hello, dex!");
		helloInter.sayHello();
	}

}
