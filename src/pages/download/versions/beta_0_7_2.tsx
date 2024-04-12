import VersionInfo from "@site/src/pages/download/versionInfo";
import Link from "@docusaurus/Link";
import useBaseUrl from "@docusaurus/useBaseUrl";
import styles from "@site/src/pages/download/index.module.css";

function Beta_0_7_2() {
    return <VersionInfo version="Beta 0.7.2"
                        date="20.07.21"
                        changes={
                            <ul>
                                <li>New nodes for pedestrians, cars, etc.</li>
                                <li>Visual effects</li>
                                <li>New objectives</li>
                                <li>Add-on LCAPI</li>
                                <li>Fixed and added new <span
                                    style={{textDecoration: "line-through"}}>bugs</span> features
                                </li>
                                <li>Many minor changes</li>
                            </ul>
                        }
                        supportOld={true}
                        buttons={[
                            <Link target="_blank" to={useBaseUrl("/downloads/Beta0.7.2.7z")}
                                  className={styles.button} download>Download LDYOM</Link>,
                            <Link target="_blank" to={useBaseUrl("/downloads/LCAPI_0.7.2.7z")}
                                  className={styles.button} download>Download LCAPI</Link>
                        ]}
                        blogLink="2021/07/20/beta-0-7-2"
                        updatedDate="07.08.21"
    />;
}

export default Beta_0_7_2;