#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <QWidget>
#include <QtWidgets>

class UserInterface : public QWidget
{
  Q_OBJECT
  auto CreateUI() -> void;

  QTextEdit * text_editor_;
  QFormLayout  * form_;
public:
  explicit UserInterface(QWidget *parent = nullptr);

signals:

public slots:
  void UpdateTextEditor(const QString header);
  void UpdateFieldsEditor(QStringList & fields);
};

#endif // USER_INTERFACE_H
