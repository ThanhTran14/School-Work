package com.revature.repository;

import com.revature.entity.User;
import com.revature.exception.LoginFail;
import com.revature.exception.UserSqlException;
import com.revature.utility.DatabaseConnector;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class SqLiteUserDao implements UserDao{
    @Override
    public User createUser(User newUser) {
        String sql = "insert into user values (?, ?)";
        try (Connection connection = DatabaseConnector.createConnection()) {
            PreparedStatement preparedStatement = connection.prepareStatement(sql);
            preparedStatement.setString(1, newUser.getUsername());
            preparedStatement.setString(2, newUser.getPassword());

            int result = preparedStatement.executeUpdate();
            if (result == 1) {
                return newUser;
            }

            throw new UserSqlException("User could not be created: please try again");
        } catch (SQLException e){
            throw new UserSqlException(e.getMessage());
        }
    }

    @Override
    public List<User> getAllUsers() {
        String sql = "select * from user";
        try (Connection connection = DatabaseConnector.createConnection()) {
            Statement statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery(sql);
            List<User> users = new ArrayList<>();
            while (resultSet.next()) {
                User userRecord = new User();
                userRecord.setUsername(resultSet.getString("username"));
                userRecord.setPassword(resultSet.getString("password"));
                users.add(userRecord);
            }
            return users;
        } catch (SQLException e) {
            throw new UserSqlException(e.getMessage());
        }
    }


}