package progteam141010;

import java.util.Scanner;
public class StonesTable {
    //Determines how many "stone" (characters) can be removed from a table (string)
    //such that the all stones are not next to the same "color" stone
    public static void main(String[] args) {
        
        Scanner scan = new Scanner(System.in);
        
        int total = scan.nextInt();
        String raw = scan.next();
        
        int skips = 0;
        int curr = 0;
        for (int i = 1; i<total; i++) {
            if(raw.charAt(curr) == raw.charAt(i)) {
                skips++;
            } else {
                curr = i;
            }
        }
        System.out.println(skips);
    }
}
