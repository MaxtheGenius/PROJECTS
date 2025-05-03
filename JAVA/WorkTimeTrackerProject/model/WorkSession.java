// Author: Maxime Pol Marcet.
// I define the structure of a single employee work session.

package model;

import java.time.LocalDateTime;

public class WorkSession {
    private String employeeId;
    private LocalDateTime start;
    private LocalDateTime end;

    public WorkSession(String employeeId, LocalDateTime start, LocalDateTime end) {
        this.employeeId = employeeId;
        this.start = start;
        this.end = end;
    }

    public String getEmployeeId() { return employeeId; }
    public LocalDateTime getStart() { return start; }
    public LocalDateTime getEnd() { return end; }

    public long getDurationMinutes() {
        return java.time.Duration.between(start, end).toMinutes();
    }

    public boolean exceedsMaxDailyHours() {
        return getDurationMinutes() > 540;
    }

    public boolean lacksBreak() {
        return getDurationMinutes() > 360 && getDurationMinutes() < 375;
    }

    public String toString() {
        return start + " - " + end + " (" + getDurationMinutes() + " min)";
    }
}
