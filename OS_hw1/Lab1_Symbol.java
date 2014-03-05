import java.util.ArrayList;

public class Lab1_Symbol {

    private ArrayList<String> symbol;
    private ArrayList<Integer> value;
    private ArrayList<Integer> module;
    private ArrayList<Boolean> used;
    private ArrayList<Boolean> error;
    private int size;

    Lab1_Symbol() { // default constructor
        symbol = new ArrayList<String>();
        value = new ArrayList<Integer>();
        module = new ArrayList<Integer>();
        used = new ArrayList<Boolean>();
        error = new ArrayList<Boolean>();
        size = 0;
    }

    public void checkSize(int defTotal, int mod, int count, int rel) {
        int temp;
        for(int i=0; i<defTotal; i++) {
            temp = value.size()-defTotal+i;
            if(value.get(temp)>mod) {
                System.out.println("Warning: Module "+count
                        +": "+symbol.get(temp)+" to big "+value.get(temp)
                        +" (max="+mod+") assume zero relative");
                value.set(temp,rel);
            }
        }
        
    }
    
    public boolean contains(String s) {
        return symbol.contains(s);
    }

    public int isSymbol(String s) {
        if (Character.isLetter(s.charAt(0))) {
            if (s.length() > 16) {
                return 2;
            }
            for (int i = 1; i < s.length(); i++) {
                if (!(Character.isLetter(s.charAt(i))
                        || Character.isDigit(s.charAt(i)))) {
                    return 1;
                }
            }
            return 0;
        } else {
            return 3;
        }
    }

    public void addSymbol(String symb, int v, int mod) {
        int symBool = isSymbol(symb);
        if (symBool == 0) {
            if (symbol.contains(symb)) {
                error.set(symbol.indexOf(symb), true);
            } else {
                symbol.add(symb);
                value.add(v);
                module.add(mod);
                used.add(false);
                error.add(false);
                size++;
            }
        } else {
            System.out.println("Unexpected Token: " + symb);
            if (symBool == 2) {
                System.out.println("Symbols can only contain up to 16 characters.");
            } else if (symBool == 1) {
                System.out.println("Symbols can only contain alphanumerics.");
            } else if (symBool == 3) {
                System.out.println("Symbols must start with an alpha character.");
            }
            System.exit(1);
        }

    }

    public boolean hasSymbol(String s) {
        if (symbol.contains(s)) {
            return true;
        } else {
            return false;
        }
    }

    public int length() {
        return size;
    }

    public int getValue(String s) {
        int index = symbol.indexOf(s);
        used.set(index, true);
        return value.get(index);
    }

    public void print() {
        for (int i = 0; i < size; i++) {
            System.out.print(symbol.get(i) + "=" + value.get(i));
            if (error.get(i)) {
                System.out.print(" Error: This variable is multiple times defined; first value used");
            }
            System.out.println();
        }
    }
    
    public void printWarning() {
        for (int i = 0; i < size; i++) {
            if (!used.get(i)) {
                System.out.println("Warning: "+symbol.get(i)
                        +" was defined in module "+module.get(i)
                        +" but never used");
            }
        }
    }
} // end public class Lab1_Symbol
