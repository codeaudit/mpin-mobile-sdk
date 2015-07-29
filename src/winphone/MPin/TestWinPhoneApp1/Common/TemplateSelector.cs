using MPinDemo.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace MPinDemo
{
    public abstract class DataTemplateSelector : ContentControl
    {
        public virtual DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            return null;
        }

        protected override void OnContentChanged(object oldContent, object newContent)
        {
            base.OnContentChanged(oldContent, newContent);

            ContentTemplate = SelectTemplate(newContent, this);
        }
    }

    public class ExistenceSelector : DataTemplateSelector
    {
        public DataTemplate UniqueTemplate
        {
            get;
            set;
        }
        public DataTemplate DuplicateTemplate
        {
            get;
            set;
        }

        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            Backend backendItem = item as Backend;
            if (backendItem != null)
            {
                if (ReadConfiguration.IsDuplicate(backendItem))
                {
                    return DuplicateTemplate;
                }
                else
                {
                    return UniqueTemplate;
                }
            }

            return base.SelectTemplate(item, container);
        }
    }
}
