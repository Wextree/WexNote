import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

/**
 * 利用反射打印一个类的全部信息,构造方法、方法、属性
 */
public class PrintMessage {
    public static void main(String[] args) {
        // 获取类的全限定类名
        String name = "domain.Student";

        try {
            // 根据全限定类名获取类的class对象
            Class clazz = Class.forName(name);
            // 获取类的父类
            Class superClass = clazz.getSuperclass();
            // 获取类的修饰符（可以用来判断类的修饰符）
            String modifiers = Modifier.toString(clazz.getModifiers());
            // 打印以上两个信息
            if (modifiers.length() > 0)
                System.out.println("The modifiers of this class is == >>  " + modifiers);
            if (superClass != null && superClass != Object.class)
                System.out.println("The super class is == >>  " + superClass.getName());

            System.out.print("\n");
            System.out.println("**************************** 类中属性的信息 ****************************");
            printFields(clazz);
            System.out.println();
            System.out.println("**************************** 类中方法的信息 ****************************");
            printMethods(clazz);
            System.out.println();
            System.out.println("**************************** 类构造方法的信息 ****************************");
            printConstructors(clazz);

        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static void printFields(Class clazz) {
        // 获取类所有的公有域
        Field[] publicField = clazz.getFields();
        // 获取该类所有的域，包括公有和私有，但是不包括超类的域
        Field[] fields = clazz.getDeclaredFields();
        for (Field field:fields) {
            // 获取属性的类型
            Class type = field.getType();
            // 获取属性名
            String name = field.getName();
            // 属性修饰符
            String modifiers = Modifier.toString(field.getModifiers());
            if (modifiers.length() > 0)
                System.out.print(modifiers + " ");

            System.out.print(type.getName() + " " + name + ";\n");
        }
    }

    private static void printMethods(Class clazz) {
        // 返回所有的方法，包括从父类继承的方法
        Method[] methods = clazz.getDeclaredMethods();
        // 返回该类当前的方法，公有
        Method[] currentMethods = clazz.getMethods();

        for (Method method: methods) {
            String modifiers = Modifier.toString(method.getModifiers());
            if (modifiers.length() > 0)
                System.out.print(modifiers + " ");
            // 获取该方法的返回值
            Class returnType = method.getReturnType();
            // 获取该方法的方法名
            String name = method.getName();
            System.out.print(returnType.getName() + " " + name + "(");

            // 获取参数类型
            Class[] paramTypes = method.getParameterTypes();
            for (int i = 0; i < paramTypes.length; i++) {
                if (i > 0)
                    System.out.print(",");

                System.out.print(paramTypes[i].getName());
            }

            System.out.println(");");
        }
    }

    private static void printConstructors(Class classzz) {
        // 返回所有构造器
        Constructor[] constructors = classzz.getDeclaredConstructors();
        // 当前类所有构造方法
        Constructor[] currentConstructors = classzz.getConstructors();
        for (Constructor constructor : currentConstructors) {
            String name = constructor.getName();
            System.out.print("  ");
            String modifiers = Modifier.toString(constructor.getModifiers());
            if (modifiers.length() > 0) {
                System.out.print(modifiers + " ");
            }

            System.out.print(name + "(");
            // 构造方法参数类型
            Class[] paramTypes = constructor.getParameterTypes();
            for (int j = 0; j < paramTypes.length; j++) {
                if (j > 0) {
                    System.out.print(", ");
                }
                // 构造方法参数类型和参数名字
                System.out.print(paramTypes[j].getName());
            }
            System.out.println(");");
        }
    }
}
