// http://vjudge.net/vjudge/contest/view.action?cid=56468#problem/A
import java.util.Scanner;
public class ProbA {
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		//BufferedReader n = new BufferedReader(new PrintStream);
		
		int[] mountains = new int[10];
		for(int i=0; i<mountains.length; i++) {
			mountains[i] = Integer.parseInt(input.next());
		}
		int[] mountains2 = mergeSort(mountains, 0, mountains.length-1);
		System.out.println(mountains2[0]);
		System.out.println(mountains2[1]);
		System.out.println(mountains2[2]);
		input.close();
		
	}
	public static int[] mergeSort(int[] arr, int front, int end) {
		//System.out.println(front + ", " + end);
		if(front == end) {
			int[] result = {arr[end]};
			return result;
		} else {
			return merge(mergeSort(arr, front, front+((end-front)/2)), 
							mergeSort(arr, front+((end-front)/2)+1, end));
		}
	}
	public static int[] merge(int[] arr1, int[] arr2) {
		int[] result = new int[arr1.length+arr2.length];
		int index1 = 0;
		int index2 = 0;
		for(int i = 0; i<result.length; i++) {
			if(index1 >= arr1.length) {
				result[i] = arr2[index2];
				index2++;
			} else if (index2 >= arr2.length) {
				result[i] = arr1[index1];
				index1++;
			} else if (arr1[index1] < arr2[index2]) {
				result[i] = arr2[index2];
				index2++;
			} else {
				result[i] = arr1[index1];
				index1++;
			}
		}
		return result;
	}

}
