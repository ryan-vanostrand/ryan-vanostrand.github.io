/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Battleships;

/**
 *
 * @author Ryan
 */
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class battleshipModel {

    private Connection conn;

    public battleshipModel() {
        try {
            conn = DriverManager.getConnection("jdbc:sqlite:C:/Users/Ryan/Downloads/battleships.db");
        } catch (SQLException e) {
            System.err.println(e.getMessage());
        }
    }

    public ArrayList<Ship> queryShipsByYear(int launched) {
        ArrayList<Ship> ships = new ArrayList<Ship>();
        try {
            String query = "select * from ships where launched = ?";
            PreparedStatement statement = conn.prepareStatement(query);
            statement.setInt(1, launched);
            ResultSet rs = statement.executeQuery();
            while (rs.next()) {
                ships.add(new Ship(rs.getString("name"), rs.getString("class"), rs.getInt("launched")));
            }
        } catch (SQLException e) {
            System.err.println(e.getMessage());
        }
        return ships;
    }

    public void insertNewClass(String className, String countryName) {
        try {
            String query = "insert into classes values (?, 'bb', ?, 10, 16, 34000)";
            PreparedStatement statement = conn.prepareStatement(query);
            statement.setString(1, className);
            statement.setString(2, countryName);
            statement.executeUpdate();
        } catch (SQLException e) {
            System.err.println(e.getMessage());
        }
    }
}