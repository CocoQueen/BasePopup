package razerdp.hook;

import android.os.Build;

import java.lang.reflect.Field;

/**
 * Created by 大灯泡 on 2018/9/3.
 */
public class PopupReflectionHelper {
    private static final String LIBNAME = "PopupNdkHook";

    static {
        System.loadLibrary(LIBNAME);
    }

    public static PopupReflectionHelper create() {
        if (Build.VERSION.SDK_INT >= 27) {
            makeHiddenApiAccessable();
        }
        return new PopupReflectionHelper();
    }

    public Object getField(String clazzName, Object target, String name) throws Exception {
        return getField(Class.forName(clazzName), target, name);
    }

    public Object getField(Class clazz, Object target, String name) throws Exception {
        Field field = clazz.getDeclaredField(name);
        field.setAccessible(true);
        return field.get(target);
    }

    public void setField(String clazzName, Object target, String name, Object value) throws Exception {
        setField(Class.forName(clazzName), target, name, value);
    }

    public void setField(Class clazz, Object target, String name, Object value) throws Exception {
        Field field = clazz.getDeclaredField(name);
        field.setAccessible(true);
        field.set(target, value);
    }

    static native void makeHiddenApiAccessable();

}
