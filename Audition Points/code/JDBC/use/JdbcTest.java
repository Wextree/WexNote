package it.jdbc.use;

import it.jdbc.domain.User;

import javax.swing.*;
import java.sql.*;

public class JdbcTest {
    public static void main(String[] args) {
        User user = new User("lisi", 16, "male");
        // 驱动设置
        String driverClassName = "com.mysql.cj.jdbc.Driver";
        // 设置连接路径
        String url = "jdbc:mysql://localhost:3306/test?useSSL=false&serverTimezone=UTC";
        // 设置用户名和密码
        String username = "root";
        String password = "110325";
        // 初始化连接，预编译语句和获取的结果
        Connection con = null;
        PreparedStatement pre = null;
        ResultSet result = null;

        try {
            // 执行驱动并且获取连接
            Class.forName(driverClassName);
            con = DriverManager.getConnection(url, username, password);

            // 设置预编译语句格式
            String sql = "INSERT INTO user VALUES(?, ?, ?)";

            pre = con.prepareStatement(sql);
            pre.setString(1, user.getName());
            pre.setInt(2, user.getAge());
            pre.setString(3, user.getGender());
            // 更新数据库
            pre.executeUpdate();

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            //关闭资源,倒关
            try {
                if(result != null) result.close();
                if(pre != null) pre.close();
                if(con != null) con.close();  //必须要关
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

    }
}
