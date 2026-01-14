# CLI Database

A lightweight command-line tool to manage an employee database.

## Usage

### Arguments

| Flag | Description |
| :--- | :--- |
| `-n` | Creates a new database file. |
| `-f <path>` | **(Required)** Specifies the path to the database file. |
| `-a "name,address,hours"` | Adds a new employee (values must be comma-separated). |
| `-l` | Lists all employees currently in the database. |

### Quick Start

```bash
# Create and add an employee
./bin/dbview -n -f data.db -a "John Doe, 123 Main St, 40"

# List employees
./bin/dbview -f data.db -l