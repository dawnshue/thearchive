/* Linker Program by Vangie Shue (vds229@nyu.edu)
 * Operating Systems, Lab #1
 * 
 * Program files:
 * - Lab1_Main.java
 * - Lab1_Symbol.java
 * - Lab1_Reader.java
 */

import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

public class Lab1_Main {

	public static void main(String[] args) throws IOException {
		// Input file is entered at command line
		if (args.length != 1) {
			System.out.println("Error, usage: Lab1_Main <input filename>");
			System.exit(1);
		}
		// String filename = "input-1";
		String filename = args[0];

		// Test that file is valid/can be read
		File filetest = new File(filename);
		if (!(filetest.isFile() && filetest.canRead())) {
			System.out.println("File <" + filename + "> invalid.");
			System.exit(1);
		}

		// "Persistent" Variables
		Lab1_Symbol symbolTable = new Lab1_Symbol();
		// Lab1_Symtol object stores symbols for symbol table to be printed
		Lab1_Reader file = new Lab1_Reader(filename);
		// Scanner object that will read the given input
		ArrayList<Integer> modules = new ArrayList<Integer>();
		// stores "relative" address of each module
		modules.add(0);

		// ***** PASS ONE ***** //
		String token; // stores token being read from input file
		int tokenInt = 0; // for storing tokens converted to integers
		// ArrayList<String> uselist = new ArrayList<String>();
		// stores uselist for each module
		String defSymbol = "";
		int defAddress = 0;
		ArrayList<String> uselist = new ArrayList<String>();
		String useSymbol = "";
		String instruction = "";
		String sym = "";
		int progAddress = 0;
		ArrayList<String> instructList = new ArrayList<String>();
		{
			instructList.add("I");
			instructList.add("A");
			instructList.add("R");
			instructList.add("E");
		}
		int defTotal = 0;
		int linenum = 0;

		while (file.hasNext()) {
			/* DEF LIST */
			token = file.next();
			linenum = file.getLineNum();
			try {
				tokenInt = Integer.parseInt(token);
				defTotal = tokenInt;
				if (tokenInt > 16) {
					parseError(file.getLineNum(), file.getOffset(token), 4);
				}
			} catch (NumberFormatException e) {
				parseError(file.getLineNum(), file.getOffset(token), 0);
			}
			for (int a = 1; a <= tokenInt; a++) {
				linenum = file.getLineNum();
				if (file.hasNext()) {
					token = file.next();
					defSymbol = token;
				} else {
					if (linenum != file.getLineNum()) {
						parseError(file.getLineNum(), file.line.length() + 1, 1);
					}
					parseError(file.getLineNum(),
							file.getOffset(token) + token.length(), 1);
				}
				if (symbolTable.isSymbol(defSymbol) != 0) {
					parseError(file.getLineNum(), file.getOffset(token), 1);
				}
				if (!file.hasNext()) {
					parseError(file.getLineNum(),
							file.getOffset(token) + token.length(), 0);
				}
				try {
					token = file.next();
					defAddress = Integer.parseInt(token);
					defAddress = defAddress + modules.get(modules.size() - 1);
					symbolTable
							.addSymbol(defSymbol, defAddress, modules.size());
				} catch (NumberFormatException e) {
					parseError(file.getLineNum(), file.getOffset(token), 0);
				}
			}

			/* USE LIST */
			if (!file.hasNext()) {
				parseError(file.getLineNum(),
						file.getOffset(token) + token.length(), 0);
			}
			token = file.next();
			try {
				tokenInt = Integer.parseInt(token);
				if (tokenInt > 16) {
					parseError(file.getLineNum(), file.getOffset(token), 5);
				}
			} catch (NumberFormatException e) {
				parseError(file.getLineNum(), file.getOffset(token), 0);
			}
			for (int b = 1; b <= tokenInt; b++) {
				if (!file.hasNext()) {
					parseError(file.getLineNum(),
							file.getOffset(token) + token.length(), 1);
				}
				token = file.next();
				useSymbol = token;
				if (symbolTable.isSymbol(useSymbol) != 0) {
					parseError(file.getLineNum(), file.getOffset(useSymbol), 1);
				}
				uselist.add(useSymbol);
			}
			/* PROGRAM TEXT */
			if (!file.hasNext()) {
				parseError(file.getLineNum(),
						file.getOffset(token) + token.length(), 0);
			}
			token = file.next();
			try {
				tokenInt = Integer.parseInt(token);
			} catch (NumberFormatException e) {
				parseError(file.getLineNum(), file.getOffset(token), 0);
			}
			modules.add(tokenInt + modules.get(modules.size() - 1));
			if (modules.get(modules.size() - 1) > 512) {
				parseError(file.getLineNum(), file.getOffset(token), 7);
			}
			symbolTable.checkSize(defTotal,
					modules.get(modules.size() - 1) - 1, modules.size() - 1,
					modules.get(modules.size() - 2));
			for (int c = 1; c <= tokenInt; c++) {
				if (file.hasNext()) {
					token = file.next();
					instruction = token;
				} else {
					parseError(file.getLineNum(),
							file.getOffset(token) + token.length(), 2);
				}
				if (!instructList.contains(instruction)) {
					parseError(file.getLineNum(), file.getOffset(token), 7);
				}
				try {
					if (!file.hasNext()) {
						parseError(file.getLineNum(), file.getOffset(token)
								+ token.length(), 2);
					}
					token = file.next();
					progAddress = Integer.parseInt(token);
					if (instruction.equals("E")) {
						if (!((uselist.size() - 1) < (progAddress % 1000))) {
							sym = uselist.get(progAddress % 1000);
							if (symbolTable.contains(sym)) {
								progAddress = progAddress
										+ symbolTable.getValue(sym);
							}
						}
					}
				} catch (NumberFormatException e) {
					parseError(file.getLineNum(), file.getOffset(token), 0);
				}
			}

			uselist.clear(); // reset it for the next module to use

		} // end of while loop1
		file.close();

		// ***** PRINTING AFTER PASS ONE ***** //
		System.out.println("Symbol Table");
		symbolTable.print();
		System.out.println();

		// ***** PASS TWO ***** //
		ArrayList<Integer> memMap = new ArrayList<Integer>();
		Lab1_Reader file2 = new Lab1_Reader(filename);
		int index = 0;
		ArrayList<Boolean> used = new ArrayList<Boolean>();

		System.out.println("Memory Map");
		int mapCount = 0;
		int modCount = 0;

		while (file2.hasNext()) {
			modCount++;
			/* DEF LIST */
			token = file2.next();
			tokenInt = Integer.parseInt(token);
			for (int a = 1; a <= tokenInt; a++) {
				file2.next(); // the symbol
				file2.next(); // the value
			}

			/* USE LIST */
			token = file2.next();
			tokenInt = Integer.parseInt(token);
			for (int b = 1; b <= tokenInt; b++) {
				useSymbol = file2.next();
				uselist.add(useSymbol);
				used.add(false);
			}

			/* PROGRAM TEXT */
			token = file2.next();
			tokenInt = Integer.parseInt(token);
			int error = 0;
			for (int c = 1; c <= tokenInt; c++) {
				instruction = file2.next();
				progAddress = Integer.parseInt(file2.next());
				/*
				 * if ((progAddress / 1000 >= 10)) { progAddress = 9999;
				 * System.out.println("Invalid opcode, set to 9999"); }
				 */
				if (instruction.equals("I")) {
					if (progAddress > 9999) {
						progAddress = 9999;
						error = 6;
					}
				} else if (instruction.equals("A")) {
					if (progAddress % 1000 > 512) {
						progAddress = progAddress - progAddress % 1000;
						error = 1;
					}
				} else if (instruction.equals("R")) {
					if (progAddress > 9999) {
						progAddress = 9999;
						error = 7;
					} else {
						progAddress = progAddress + modules.get(index);
						if (progAddress % 1000 > modules
								.get(modules.size() - 1)) {
							progAddress = progAddress - progAddress % 1000;
							progAddress = progAddress + modules.get(index);
							error = 2;
						}
					}
				} else if (instruction.equals("E")) {
					if (!((uselist.size() - 1) < (progAddress % 1000))) {
						sym = uselist.get(progAddress % 1000);
						used.set(progAddress % 1000, true);
						progAddress = progAddress - progAddress % 1000;
						if (symbolTable.contains(sym)) {
							progAddress = progAddress
									+ symbolTable.getValue(sym);
						} else {
							error = 4;
						}
					} else {
						error = 3;
					}
				}

				// Save memory address & Print to screen/output
				memMap.add(progAddress);
				System.out.print(String.format("%03d", mapCount));
				System.out.print(": ");
				System.out.print(String.format("%04d", progAddress));
				errMessage(error, sym);
				System.out.println();
				error = 0;
				mapCount++;
			}
			index++; // increment for the next module
			for (int i = 0; i < used.size(); i++) {
				if (!used.get(i)) {
					System.out.println("Warning: In Module " + modCount + " "
							+ uselist.get(i) + " appeared in the uselist"
							+ " but was not actually used");
				}
			}
			uselist.clear();
			used.clear();
		} // end of while loop2
		file2.close();

		System.out.println();
		symbolTable.printWarning();

	} // End of main method

	public static void parseError(int linenum, int lineoffset, int errcode) {
		String errstr[] = { "NUM_EXPECTED", "SYM_EXPECTED", "ADDR_EXPECTED",
				"SYM_TOLONG", "TO_MANY_DEF_IN_MODULE", "TO_MANY_USE_IN_MODULE",
				"TO_MANY_SYMBOLS", "TO_MANY_INSTR", };
		System.out.println("Parse Error line " + linenum + " offset "
				+ lineoffset + ": " + errstr[errcode]);
		System.exit(1);
	}

	public static void errMessage(int errcode, String sym) {
		String errstr[] = {
				"",
				"Absolute address exceeds machine size; zero used",
				"Relative address exceeds module size; zero used",
				"External address exceeds length of uselist; treated as immediate",
				sym + " is not defined; zero used",
				"This variable is multiple times defined; first value used",
				"Illegal immediate value; treated as 9999",
				"Illegal opcode; treated as 9999" };
		if (errcode > 0) {
			System.out.print(" Error: " + errstr[errcode]);
		}
	}

	public static void warning(int errcode, int d, String s) {
		String errstr[] = {
				"Module " + d + ": " + s + " to big " + d + " (max=" + d
						+ ") assume zero relative",
				"In module " + d + " " + s
						+ "appeared in the uselist but was not actually used",
				s + " was defined in module " + d + " but never used" };
		System.out.println("Warning: " + errstr[errcode]);
	}

	public static void syntaxError(int line, int type) {
		switch (type) {
		case 1:
			System.out.print("Line " + line + ": ");
			System.out.println("Syntax error, numeric count expected.");
			break;
		case 2:
			System.out.print("Line " + line + ": ");
			System.out.println("Syntax error, missing definition.");
			break;
		case 3:
			System.out.print("Line " + line + ": ");
			System.out.println("Syntax error, missing uselist token.");
			break;
		case 4:
			System.out.print("Line " + line + ": ");
			System.out.println("Syntax error, invalid instruction type. "
					+ "Only {I, A, R, E} are valid");
			break;
		case 5:
			System.out.print("Line " + line + ": ");
			System.out
					.println("Syntax error, insufficient uselist symbols defined");
		case 6:
			System.out.print("Line " + line + ": ");
			System.out.println("Syntax error, numeric address expected.");
			break;
		}
		System.exit(1);
	}


} // End of public class Lab1_Main
