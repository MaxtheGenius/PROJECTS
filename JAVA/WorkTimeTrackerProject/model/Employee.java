// Author: Maxime Pol Marcet.
// I define the structure of an employee.

package model;

public class Employee {
    private String id;
    private String name;
    private boolean isRemote;

    public Employee(String id, String name, boolean isRemote) {
        this.id = id;
        this.name = name;
        this.isRemote = isRemote;
    }

    public String getId() { return id; }
    public String getName() { return name; }
    public boolean isRemote() { return isRemote; }

    public String toString() {
        return name + " (ID: " + id + ", Remote: " + isRemote + ")";
    }
}
