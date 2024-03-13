#include "data_def.h"

extern bool cfgEquals(const ProcessConfiguration &lhs,
                      const ProcessConfiguration &rhs) {
  bool ret = true;

  if (lhs.id != rhs.id) return false;
  if (lhs.alias != rhs.alias) return false;
  if (lhs.process_name != rhs.process_name) return false;
  if (lhs.process_path != rhs.process_path) return false;

  if (lhs.process_arguments.count() != rhs.process_arguments.count())
    return false;
  for (int i = 0; i < lhs.process_arguments.count(); i++) {
    if (lhs.process_arguments.value(i) != rhs.process_arguments.value(i))
      return false;
  }

  if (lhs.window_title != rhs.window_title) return false;
  if (lhs.wnd_class_name != rhs.wnd_class_name) return false;
  if (lhs.waiting_time != rhs.waiting_time) return false;
  if (lhs.kill_and_start != rhs.kill_and_start) return false;

  return ret;
}

extern QStringList GetArguments(const QString &para) {
  // 位置列表
  QList<int> posList;
  int pos = 0;
  do {
    pos = para.indexOf('"', pos);
    if (pos != -1) {
      posList << pos;
      pos += 1;
    }
  } while (pos != -1);

  // 成对的位置
  QMap<int, int> posMap;
  for (int i = 0; i < posList.count() - 1; (++i)++) {
    posMap.insert(posList.value(i), posList.value(i + 1));
  }

  // 获取引号括起来的字符串列表
  QStringList quoteList;
  for (auto it = posMap.cbegin(); it != posMap.cend(); ++it) {
    auto str = para.mid(it.key(), it.value() - it.key() + 1);
    qInfo() << "SB:" << str;
    quoteList << str;
  }

  // 原来的字符串去掉引号括起来的字符串
  auto tempPara = para;
  qInfo() << "Father:" << para;
  for (auto item : quoteList) {
    tempPara.remove(item);
  }
  // 去掉一些引号
  tempPara = tempPara.remove('"').trimmed();
  qInfo() << "SON:" << tempPara;

  QStringList ret(quoteList);
  ret.append(tempPara.split(' ', Qt::SkipEmptyParts));

  return ret;
};

extern QStringList GetArgumentsOrdered(const QString &para) {
  QStringList ret;

  // 空
  if (para.isEmpty()) {
    return ret;
  }

  // 一个参数
  if (para.indexOf(' ') == -1 || para.indexOf('"') == -1) {
    ret << para;
    return ret;
  }

  int normalCharPos = -1;
  int quoteCharPos = -1;
  for (int i = 0; i < para.count(); i++) {
    auto oneChar = para.at(i);
    if (oneChar.isSpace()) {
      if (quoteCharPos != -1) {
        continue;
      } else {
        if (normalCharPos != -1) {
          auto str = para.mid(normalCharPos, i - normalCharPos);
          ret << str;
          normalCharPos = -1;
        } else {
          // 连续空格
        }
        continue;
      }
    }

    if (oneChar == '"') {
      if (quoteCharPos == -1) {
        // 引号开始
        quoteCharPos = i;
      } else {
        // 引号结束
        auto str = para.mid(quoteCharPos + 1, i - quoteCharPos - 1);
        ret << str;
        quoteCharPos = -1;
      }
      continue;
    }

    if (normalCharPos != -1) {
      continue;
    }

    if (quoteCharPos != -1) {
      continue;
    }

    // 非 ' '和'"'字符 开始
    normalCharPos = i;
  }

  return ret;
};
