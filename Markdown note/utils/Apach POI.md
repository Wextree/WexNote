# Apache POI 

> 基于Apache POI的Java读写excel的操作实现

```xml
<dependencies>
        <!-- https://mvnrepository.com/artifact/org.apache.poi/poi -->
        <dependency>
            <groupId>org.apache.poi</groupId>
            <artifactId>poi-ooxml</artifactId>
            <version>3.17</version>
        </dependency>

<!--        <dependency>-->
<!--            <groupId>net.sourceforge.jexcelapi</groupId>-->
<!--            <artifactId>jxl</artifactId>-->
<!--            <version>2.6.12</version>-->
<!--        </dependency>-->

        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.18.12</version>
        </dependency>
    </dependencies>
```



```java
package utils;

import domain.ExcelOutPut;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.util.NumberToTextConverter;
import org.apache.poi.xssf.usermodel.XSSFCell;
import org.apache.poi.xssf.usermodel.XSSFRow;
import org.apache.poi.xssf.usermodel.XSSFSheet;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

public class PoiHelper {

    /**
     * 读取xlsx文件
     *
     * @param io 文件绝对路径
     * @throws Exception
     */
    public static ExcelOutPut readXlsx(InputStream io) throws Exception {
        ArrayList<ArrayList<String>> dataList = new ArrayList<>();
        ArrayList<String> titleList = new ArrayList<>();

        XSSFWorkbook xssfworkbook = new XSSFWorkbook(io);
        // 获取第一个sheet
        XSSFSheet xssfSheet = xssfworkbook.getSheetAt(0);

        int rowStart = xssfSheet.getFirstRowNum();
        int rowEnd = xssfSheet.getLastRowNum();

        // 分离excel第一行
        XSSFRow row1 = xssfSheet.getRow(rowStart);
        if (null == row1){
            xssfworkbook.close();
            return null;
        }
        int cellStart1 = row1.getFirstCellNum();
        int cellEnd1 = row1.getLastCellNum();
        for (int i = cellStart1; i <= cellEnd1; i++) {
            XSSFCell cell = row1.getCell(i);
            if (null == cell){
                // 如果该格为空
                titleList.add("");
            }else {
                titleList.add(cell.toString());
            }
        }

        // 获取正式数据
        for (int i = rowStart+1; i <= rowEnd ; i++) {
            XSSFRow row = xssfSheet.getRow(i);
            if (null == row)
                continue;

            int cellStart = row.getFirstCellNum();
            int cellEnd = row.getLastCellNum();
            ArrayList<String> list = new ArrayList<>();

            for (int j = cellStart; j <= cellEnd; j++) {
                XSSFCell cell = row.getCell(j);
                if (null == cell){
                    list.add("");
                }else {
                    // 判断该格的类型，好做转换
                    switch (cell.getCellTypeEnum()){
                        case NUMERIC: // 数字
                            list.add(NumberToTextConverter.toText(cell.getNumericCellValue()));
                            break;
                        case STRING: // 字符串
                            list.add(cell.getStringCellValue().trim());
                            break;
                        case BOOLEAN: // 布尔值
                            list.add(String.valueOf(cell.getBooleanCellValue()));
                            break;
                        case FORMULA: // 公式
                            list.add(cell.getCellFormula());
                            break;
                        case BLANK: // 空值
                            list.add("");
                            break;
                        case ERROR: // 故障
                            list.add("");
                            break;
                        default:
                            list.add("");
                            break;
                    }
                }
            }
            dataList.add(list);
        }
        ExcelOutPut excelOutPut = new ExcelOutPut(titleList, dataList);
        xssfworkbook.close();
        return excelOutPut;
    }

    /**
     * 读取xls文件
     *
     * @param io 文件绝对路径
     * @throws Exception
     */
    public static ExcelOutPut readXls(InputStream io) throws Exception {
        // 只是在这里的格式不一样而已，其他是相同的
        HSSFWorkbook hssfWorkbook = new HSSFWorkbook(io);
        HSSFSheet hssfSheet = hssfWorkbook.getSheetAt(0);
        // ......
        return null;
    }

    /**
     * 使用POI创建excel工作簿
     *
     * @param excelOutPut 输入数据
     * @throws IOException
     */
    public static boolean createExcel(ExcelOutPut excelOutPut) throws IOException {
        ArrayList<String> titleList = excelOutPut.getTitleList();
        ArrayList<ArrayList<String>> dataList = excelOutPut.getDataList();
        // 创建excel工作簿
        XSSFWorkbook wb = new XSSFWorkbook();
        // 创建第一个sheet（页），命名为 new sheet
        XSSFSheet sheet = wb.createSheet();
         wb.setSheetName(0, "sheet1");// 工作簿名称
        // Row 行
        // Cell 方格
        // Row 和 Cell 都是从0开始计数的

        // 创建一行，在页sheet上
        XSSFRow row = sheet.createRow((short) 0);
        for (int i = 0; i < titleList.size(); i++) {
            // 在row行上创建一个方格,设置方格的显示
            row.createCell(i).setCellValue(titleList.get(i));
        }

        for (int i = 0; i < dataList.size(); i++) {
            row = sheet.createRow((short) i + 1);
            for (int j = 0; j < dataList.get(i).size(); j++) {
                row.createCell(j).setCellValue(dataList.get(i).get(j));
            }
        }

        wb.write(new FileOutputStream("test.xlsx"));
        wb.close();
        return true;
    }
}
```