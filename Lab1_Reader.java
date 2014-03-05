import java.io.*;
import java.util.Scanner;

public class Lab1_Reader {

	private Scanner file;
	private Scanner tokenizer;
	private String filename;
	private int linenum;
	public String line;
	private String[] tokens;

	Lab1_Reader(String input) {
		try {
			filename = input;
			// file = new Scanner(new BufferedReader(new FileReader(filename)));
			file = new Scanner(new FileReader(filename));
			// reader = new BufferedReader(new FileReader(filename));
			linenum = 0;
			if (file.hasNextLine()) {
				line = file.nextLine();
				tokenizer = new Scanner(line);
			}
			linenum = 1;
		} catch (IOException ioe) {
			System.out.println("File <" + filename + "> not found.");
			System.out.println(ioe);
			System.exit(1);
		}
	}

	public int getLineNum() {
		return linenum;
	}

	public int getOffset(String token) {
		return (line.indexOf(token) + 1);
	}

	public boolean hasNext() {
		if (tokenizer.hasNext()) {
			return true;
		}
		while (!tokenizer.hasNext()) {
			if (!file.hasNextLine()) {
				return false;
			}
			line = file.nextLine();
			tokenizer.close();
			tokenizer = new Scanner(line);
			linenum++;
		}
		return true;
	}

	public String next() {
		if (hasNext()) {
			return tokenizer.next();
		} else {
			System.out.println("NONEXT");
			return "NONEXT";
		}
	}

	public void close() {
		file.close();
	}
}
