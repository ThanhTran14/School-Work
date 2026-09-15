package com.revature.service;

import com.revature.entity.User;
import com.revature.exception.LoginFail;
import com.revature.exception.ValidateFail;
import com.revature.repository.UserDao;

import java.util.List;

public class UserService {

    private UserDao userDao;

    public UserService(UserDao userDao) {
        this.userDao = userDao;
    }

    private boolean checkPassword(User newUser){
        boolean usernameValid = newUser.getUsername().length() <= 30;
        boolean passwordValid = newUser.getPassword().length() <= 30;
        return usernameValid && passwordValid;
    }

    private boolean usernameUnique(User newUser) {
        boolean usernameIsUnique = true;
        List<User> users = userDao.getAllUsers();
        for(User user : users){
            if(newUser.getUsername().equals(user.getUsername())){
                usernameIsUnique = false;
                break;
            }
        }
        return usernameIsUnique;
    }


    public User validate(User newCredentials) {
        if (checkPassword(newCredentials)) {
            if (usernameUnique(newCredentials)) {
                return userDao.createUser(newCredentials);
            }

        }
        throw new ValidateFail("Could not validate your account");
    }

    public User checkLogin(User credentials) {
        for (User user : userDao.getAllUsers()) {
            boolean usernameMatch = user.getUsername().equals(credentials.getUsername());
            boolean passwordMatch = user.getPassword().equals(credentials.getPassword());
            if (usernameMatch && passwordMatch) {
                return credentials;
            }
        }
        throw new LoginFail("Credentials are invalid: Please try again");
    }


}