//
// This file was generated by the JavaTM Architecture for XML Binding(JAXB) Reference Implementation, vJAXB 2.1.10 in JDK 6 
// See <a href="http://java.sun.com/xml/jaxb">http://java.sun.com/xml/jaxb</a> 
// Any modifications to this file will be lost upon recompilation of the source schema. 
// Generated on: 2010.08.07 at 12:54:52 AM CEST 
//


package mccf.definition;

import java.util.ArrayList;
import java.util.List;
import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlSchemaType;
import javax.xml.bind.annotation.XmlType;


/**
 * <p>Java class for ChoiceSetting complex type.
 * 
 * <p>The following schema fragment specifies the expected content contained within this class.
 * 
 * <pre>
 * &lt;complexType name="ChoiceSetting">
 *   &lt;complexContent>
 *     &lt;extension base="{urn:strike-devices:configuration}ChoiceSetting">
 *       &lt;redefine>
 *         &lt;complexType name="ChoiceSetting">
 *           &lt;complexContent>
 *             &lt;restriction base="{http://www.w3.org/2001/XMLSchema}anyType">
 *               &lt;sequence>
 *                 &lt;group ref="{urn:strike-devices:configuration}Infos"/>
 *                 &lt;element name="image" type="{urn:strike-devices:configuration}DescriptionImage" minOccurs="0"/>
 *                 &lt;element name="option" type="{urn:strike-devices:configuration}Option" maxOccurs="unbounded"/>
 *               &lt;/sequence>
 *               &lt;attribute name="default" type="{http://www.w3.org/2001/XMLSchema}IDREF" />
 *               &lt;attribute name="prefix" type="{urn:strike-devices:configuration}SettingID" />
 *             &lt;/restriction>
 *           &lt;/complexContent>
 *         &lt;/complexType>
 *       &lt;/redefine>
 *       &lt;attGroup ref="{urn:strike-devices:configuration}Position"/>
 *       &lt;attribute name="bit-width" use="required" type="{http://www.w3.org/2001/XMLSchema}unsignedInt" />
 *       &lt;attribute name="byte-dividers">
 *         &lt;simpleType>
 *           &lt;list itemType="{http://www.w3.org/2001/XMLSchema}unsignedInt" />
 *         &lt;/simpleType>
 *       &lt;/attribute>
 *     &lt;/extension>
 *   &lt;/complexContent>
 * &lt;/complexType>
 * </pre>
 * 
 * 
 */
@XmlAccessorType(XmlAccessType.FIELD)
@XmlType(name = "ChoiceSetting")
public class ChoiceSetting
    extends OriginalChoiceSetting
{

    @XmlAttribute(name = "bit-width", required = true)
    @XmlSchemaType(name = "unsignedInt")
    protected long bitWidth;
    @XmlAttribute(name = "byte-dividers")
    protected List<Long> byteDividers;
    @XmlAttribute(name = "byte-no", required = true)
    @XmlSchemaType(name = "unsignedInt")
    protected long byteNo;
    @XmlAttribute(name = "bit-no", required = true)
    @XmlSchemaType(name = "unsignedInt")
    protected long bitNo;

    /**
     * Gets the value of the bitWidth property.
     * 
     */
    public long getBitWidth() {
        return bitWidth;
    }

    /**
     * Sets the value of the bitWidth property.
     * 
     */
    public void setBitWidth(long value) {
        this.bitWidth = value;
    }

    /**
     * Gets the value of the byteDividers property.
     * 
     * <p>
     * This accessor method returns a reference to the live list,
     * not a snapshot. Therefore any modification you make to the
     * returned list will be present inside the JAXB object.
     * This is why there is not a <CODE>set</CODE> method for the byteDividers property.
     * 
     * <p>
     * For example, to add a new item, do as follows:
     * <pre>
     *    getByteDividers().add(newItem);
     * </pre>
     * 
     * 
     * <p>
     * Objects of the following type(s) are allowed in the list
     * {@link Long }
     * 
     * 
     */
    public List<Long> getByteDividers() {
        if (byteDividers == null) {
            byteDividers = new ArrayList<Long>();
        }
        return this.byteDividers;
    }

    /**
     * Gets the value of the byteNo property.
     * 
     */
    public long getByteNo() {
        return byteNo;
    }

    /**
     * Sets the value of the byteNo property.
     * 
     */
    public void setByteNo(long value) {
        this.byteNo = value;
    }

    /**
     * Gets the value of the bitNo property.
     * 
     */
    public long getBitNo() {
        return bitNo;
    }

    /**
     * Sets the value of the bitNo property.
     * 
     */
    public void setBitNo(long value) {
        this.bitNo = value;
    }

}
