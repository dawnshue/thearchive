import java.util.ArrayList;
import java.util.Scanner;
import java.lang.String;

/**
 * NYU Entry Problems 5 - Valera & X
 * @author Vangie
 */
public class Valera {

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        int N = input.nextInt();

        String[] lines = new String[N];
        input.nextLine();
        for(int i=0; i<N; i++) {
            lines[i] = input.nextLine();
            //System.out.println(i);
        }

        char YES, NO;
        YES = lines[0].charAt(0);
        NO = lines[0].charAt(1);

        boolean ANS = true;
        for (int x = 0; x < N; x++) { //Iterate down the rows (lines)
            for (int y = 0; y < N; y++) { //Iterate along chars of a line
                if (y == x || y == (N - x - 1)) {
                    if (YES != lines[x].charAt(y)) {
                        ANS = false;
                        break;
                    }
                } else {
                    if (NO != lines[x].charAt(y)) {
                        ANS = false;
                        break;
                    }
                }
            }
            if (!ANS) {
                break;
            }
        }
        if (ANS) {
            System.out.print("YES");
        } else {
            System.out.print("NO");
        }
        
        
    }

}
