import java.util.Scanner;

/*
 * NYU Entry Problems 6
 * Problem A - Mashmokh & Lights
 */

public class Mashmokh {

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		
		int totLights = input.nextInt();
		//System.out.println("TotLights: "+totLights);
		int[] lights = new int[totLights];
		for (int a = 0; a < totLights; a++) {
			lights[a] = 0;
		}
		
		int buttons = input.nextInt();
		//System.out.println("buttons: "+buttons);
		
		int nextButton;
		for (int a = 0; a < buttons; a++) {
			nextButton = input.nextInt();
			//System.out.println("NextButton: "+nextButton);
			for (int b = nextButton - 1; b < totLights; b++) {
				if (lights[b] == 0) {
					lights[b] = nextButton;
				} else {
					break;
				}
			}
		}
		
		for (int a = 0; a < totLights; a++) {
			System.out.print(lights[a]);
			System.out.print(" ");
		}
		
		input.close();

	}

}
