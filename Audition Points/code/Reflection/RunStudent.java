import domain.Student;

import java.lang.reflect.Field;

/**
 * 在运行时使用反射分析对象
 */
public class RunStudent {
    public static void main(String[] args) {
        Student student = new Student("zhangsan", 23, true, 2000.0);
        printFieldValue(student);
    }

    private static void printFieldValue(Student student) {
        Class clazz = student.getClass();
        Field[] fields = clazz.getDeclaredFields();

        for (Field field:fields) {
            String name = field.getName();
            try {
                Field property = clazz.getDeclaredField(name);
                // 尝试设置权限可以访问私有域
                property.trySetAccessible();
                Object fieldValue = property.get(student);
                System.out.println("[fileName == >" + name + "],[" + "fieldValue == >" + fieldValue + "]");
            } catch (NoSuchFieldException | IllegalAccessException e) {
                e.printStackTrace();
            }

        }
    }
}
