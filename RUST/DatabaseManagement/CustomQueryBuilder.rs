use std::collections::HashMap;

#[derive(Debug)]
pub struct QueryBuilder {
    table: String,
    conditions: HashMap<String, String>,
    columns: Vec<String>,
}

impl QueryBuilder {
    pub fn new(table: &str) -> Self {
        QueryBuilder {
            table: table.to_string(),
            conditions: HashMap::new(),
            columns: vec!["*".to_string()],
        }
    }

    pub fn select(&mut self, columns: Vec<String>) -> &mut Self {
        self.columns = columns;
        self
    }

    pub fn where_eq(&mut self, field: &str, value: &str) -> &mut Self {
        self.conditions.insert(field.to_string(), value.to_string());
        self
    }

    pub fn build(&self) -> String {
        let mut query = format!("SELECT {} FROM {}", self.columns.join(", "), self.table);
        
        if !self.conditions.is_empty() {
            query.push_str(" WHERE ");
            let conditions = self.conditions.iter()
                .map(|(key, value)| format!("{} = '{}'", key, value))
                .collect::<Vec<String>>()
                .join(" AND ");
            query.push_str(&conditions);
        }

        query
    }
}