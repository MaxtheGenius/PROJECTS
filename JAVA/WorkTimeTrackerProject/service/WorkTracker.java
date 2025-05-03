// Author: Maxime Pol Marcet.
// I manage all employees and sessions, and generate compliance reports.

package service;

import model.Employee;
import model.WorkSession;
import java.time.LocalDate;
import java.util.*;

public class WorkTracker {
    private Map<String, Employee> employees = new HashMap<>();
    private List<WorkSession> sessions = new ArrayList<>();

    public void registerEmployee(String id, String name, boolean isRemote) {
        employees.put(id, new Employee(id, name, isRemote));
    }

    public void logSession(String employeeId, LocalDateTime start, LocalDateTime end) {
        sessions.add(new WorkSession(employeeId, start, end));
    }

    public String getGuiReport() {
        StringBuilder sb = new StringBuilder();
        for (String empId : employees.keySet()) {
            Employee emp = employees.get(empId);
            sb.append("Employee: ").append(emp).append("\n");
            for (WorkSession s : sessions) {
                if (s.getEmployeeId().equals(empId)) {
                    sb.append("  ").append(s).append("\n");
                    if (s.exceedsMaxDailyHours()) sb.append("    [!] Exceeds 9h.\n");
                    if (s.lacksBreak()) sb.append("    [!] No break.\n");
                }
            }
            sb.append("\n");
        }
        return sb.toString();
    }
}
